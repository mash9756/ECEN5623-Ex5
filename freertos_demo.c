/**
 *  @name   freertos_demo.c
 *  @brief  freertos_demo adaptation for simple cyclic-executive system
 * 
 *  @author Mark Sherman
*/

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#include "services.h"
#include "sequencer.h"

#define SYS_CLK         (50000000)
/* 30Hz timer has a period of ~33ms (1 / 30 = .033s) */
#define TIMER_30HZ      (SYS_CLK / 1000 * 33)
/* 3000Hz timer has a period of ~333us (1 / 3000 = .000333s) */
#define TIMER_3000HZ    (SYS_CLK / 1000000 * 33)

static uint8_t ui8IntCnt = 0;

xSemaphoreHandle g_pUARTSemaphore;
xSemaphoreHandle g_pSx[NUM_OF_SERVICES];

bool abortSx[NUM_OF_SERVICES];
bool releaseSx[NUM_OF_SERVICES];
uint32_t SxCnt[NUM_OF_SERVICES];

// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

// This hook is called by FreeRTOS when an stack overflow error is detected.
void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName) {
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}

/**
 *  @name   ConfigureUART0
 *  @brief  setup hardware UART0 peripheral
*/
void ConfigureUART0(void) {
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

/**
 *  @name   ConfigureTimer0
 *  @brief  setup hardware Timer0 peripheral
*/
void ConfigureTimer0(void) {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Enable processor interrupts.
    ROM_IntMasterEnable();

    ROM_IntPrioritySet(INT_TIMER0A, 0x01);

    // Configure the two 32-bit periodic timers.
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, TIMER_30HZ);

    // Setup the interrupts for the timer timeouts.
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}


//*****************************************************************************
// The interrupt handler for the first timer interrupt.
// Sequencer = RT_MAX	@ 30 Hz
// Servcie_1 = RT_MAX-1	@ 3 Hz      10
// Service_2 = RT_MAX-2	@ 1 Hz      30
// Service_3 = RT_MAX-3	@ 0.5 Hz    60
// Service_4 = RT_MAX-2	@ 1 Hz      30
// Service_5 = RT_MAX-3	@ 0.5 Hz    60
// Service_6 = RT_MAX-2	@ 1 Hz      30
// Service_7 = RT_MIN	0.1 Hz      300
//*****************************************************************************
void Timer0IntHandler(void) {
/* Clear Timer0 interrupt immediately to prevent repeated triggering */
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    ui8IntCnt++;

    if(ui8IntCnt % 10 == 0) {
        releaseSx[S1] = true;
    }
    if(ui8IntCnt % 30 == 0) {
        releaseSx[S2] = true;
        releaseSx[S4] = true;
        releaseSx[S6] = true;
    }
    if(ui8IntCnt % 60 == 0) {
        releaseSx[S3] = true;
        releaseSx[S5] = true;
    }
    if(ui8IntCnt % 300 == 0) {
        releaseSx[S7] = true;
        ui8IntCnt = 0;
    }
    
    xSemaphoreGiveFromISR(g_pSx[SEQ], NULL);

    // portTickType message;
    // message = xTaskGetTickCount();
    // if(xQueueSend(g_pComp1Queue, &message, portMAX_DELAY) != pdPASS) {
    // /* Queue should never be full. If so print the error message on UART and wait for ever. */
    //     UARTprintf("\nQueue full. This should never happen.\n");
    // }
    // xSemaphoreGiveFromISR(g_pTimer0Semaphore, NULL);
}


//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************
int main(void)
{
    uint8_t i   = S1;
    int ret     = 0;

/* configure clock source to 50MHz using PLL */
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    ROM_FPULazyStackingEnable();

/* Initialize hardware UART0 and configure it for 115,200, 8-N-1 operation. */
    ConfigureUART0();

/* Initialize hardware Timer0 peripheral and interrupts */
    ConfigureTimer0();

    UARTprintf("\n\n--- ECEN5623 Exercise 5 ---\n");

/* create UART mutex to protect writes */
    g_pUARTSemaphore = xSemaphoreCreateMutex();

/* initialize service control variables and semaphores */
    for(; i < NUM_OF_SERVICES ; i++) {
        abortSx[i]      = false;
        releaseSx[i]    = false;
        SxCnt[i]        = 0;
        
        g_pSx[i] = xSemaphoreCreateMutex();
        xSemaphoreTake(g_pSx[i], portMAX_DELAY);
    }

/* create sequencer */
    ret = SequencerInit();
    if(ret) {
        UARTprintf("Failed to create Sequencer: %d\n", ret);
        return -1;
    }

    ret = Service1Init();
    if(ret)
    {
        UARTprintf("Failed to create Service 1: %d\n", ret);
        return -1;
    }

    ret = Service2Init();
    if(ret)
    {
        UARTprintf("Failed to create Service 2: %d\n", ret);
        return -1;
    }

    ret = Service3Init();
   if(ret)
   {
       UARTprintf("Failed to create Service 3: %d\n", ret);
       return -1;
   }

    ret = Service4Init();
   if(ret)
   {
       UARTprintf("Failed to create Service 4: %d\n", ret);
       return -1;
   }

    ret = Service5Init();
   if(ret)
   {
       UARTprintf("Failed to create Service 5: %d\n", ret);
       return -1;
   }

    ret = Service6Init();
   if(ret)
   {
       UARTprintf("Failed to create Service 6: %d\n", ret);
       return -1;
   }

    ret = Service7Init();
   if(ret)
   {
       UARTprintf("Failed to create Service 7: %d\n", ret);
       return -1;
   }

    // Enable the timers.
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    while(1){
    // In case the scheduler returns for some reason, loop
    }
}
