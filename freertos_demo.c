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
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O.
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

    // Configure the two 32-bit periodic timers.
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, TIMER_30HZ);

    // Setup the interrupts for the timer timeouts.
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

void Timer0IntHandler(void) {
/* Clear Timer0 interrupt immediately to prevent repeated triggering */
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    ui8IntCnt++;


    Services_t *servs = xGetServices();

/* set release flag for each service as needed */
    if(ui8IntCnt % 10 == 0) {
        servs[S1].release = true;
    }
    if(ui8IntCnt % 30 == 0) {
        servs[S2].release = true;
        servs[S4].release = true;
        servs[S6].release = true;
    }
    if(ui8IntCnt % 60 == 0) {
        servs[S3].release = true;
        servs[S5].release = true;
    }
    if(ui8IntCnt % 300 == 0) {
       servs[S7].release = true;
       ui8IntCnt = 0;
    }
/* run the sequencer */
   xSemaphoreGiveFromISR(servs[SEQ].sem, NULL);
}

int main(void)
{
    int ret = 0;

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

/* create sequencer */
    ret = SequencerInit();
    if(ret) {
        UARTprintf("Failed to create Sequencer: %d\n", ret);
        return -1;
    }

    ret = uiInitServices();
    if(ret) {
        UARTprintf("Failed to create services: %d\n", ret);
        return -1;
    }

/* enable (start) the configured timer */
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);

/* start FreeRTOS scheduler, should never return */
    vTaskStartScheduler();

    while(1){
        /* loop forever on scheduler return for debugging */
    }
}
