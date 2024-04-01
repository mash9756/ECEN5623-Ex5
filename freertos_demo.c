//*****************************************************************************
//
// freertos_demo.c - Simple FreeRTOS example.
//
// Copyright (c) 2012-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.0 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

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

#define SYS_CLK     (16000000)
/* 30Hz timer has a period of ~33ms (1 / 30 = .033s) */
#define TIMER_30HZ  (SYS_CLK / 1000 * 33)

static uint8_t ui8IntCnt = 0;

//*****************************************************************************
//
// The mutex that protects concurrent access of UART from multiple tasks.
//
//*****************************************************************************
xSemaphoreHandle g_pUARTSemaphore;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
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

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void ConfigureUART(void) {
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

//*****************************************************************************
// The interrupt handler for the first timer interrupt.
//*****************************************************************************
void Timer0IntHandler(void) {
/* Clear Timer0 interrupt immediately to prevent repeated triggering */
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    ui8IntCnt++;
// Sequencer = RT_MAX	@ 30 Hz
// Servcie_1 = RT_MAX-1	@ 3 Hz      10
// Service_2 = RT_MAX-2	@ 1 Hz      30
// Service_3 = RT_MAX-3	@ 0.5 Hz    60
// Service_4 = RT_MAX-2	@ 1 Hz      30
// Service_5 = RT_MAX-3	@ 0.5 Hz    60
// Service_6 = RT_MAX-2	@ 1 Hz      30
// Service_7 = RT_MIN	0.1 Hz      300
    if(ui8IntCnt == 10) {
        releaseSx[S1] = true;
        xSemaphoreGiveFromISR(g_pSxSema[S1], NULL);
    }
    if(ui8IntCnt == 30) {
        releaseSx[S2] = true;
        releaseSx[S4] = true;
        releaseSx[S6] = true;
        xSemaphoreGiveFromISR(g_pSxSema[S2], NULL);
        xSemaphoreGiveFromISR(g_pSxSema[S4], NULL);
        xSemaphoreGiveFromISR(g_pSxSema[S6], NULL);
    }
    if(ui8IntCnt == 60) {
        releaseSx[S3] = true;
        releaseSx[S5] = true;
        xSemaphoreGiveFromISR(g_pSxSema[S3], NULL);
        xSemaphoreGiveFromISR(g_pSxSema[S5], NULL);
    }
    if(ui8IntCnt == 300) {
        releaseSx[S7] = true;
        ui8IntCnt = 0;
        xSemaphoreGiveFromISR(g_pSxSema[S7], NULL);
    }

    // portTickType message;
    // message = xTaskGetTickCount();
    // if(xQueueSend(g_pComp1Queue, &message, portMAX_DELAY) != pdPASS) {
    // /* Queue should never be full. If so print the error message on UART and wait for ever. */
    //     UARTprintf("\nQueue full. This should never happen.\n");
    // }
    // xSemaphoreGiveFromISR(g_pTimer0Semaphore, NULL);
}

void ConfigureTimer0(void) {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

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
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************
int main(void)
{
    ROM_FPULazyStackingEnable();

    // Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    // Initialize the UART and configure it for 115,200, 8-N-1 operation.
    ConfigureUART();
    
    ConfigureTimer0();

    // Print demo introduction.
    UARTprintf("\n\n--- ECEN5623 Exercise 5 ---\n");

/* create UART mutex to protect writes */
    g_pUARTSemaphore    = xSemaphoreCreateMutex();

/* create service release mutexes */
    // g_pS1Sema   = xSemaphoreCreateMutex();
    // g_pS2Sema   = xSemaphoreCreateMutex();
    // g_pS3Sema   = xSemaphoreCreateMutex();
    // g_pS4Sema   = xSemaphoreCreateMutex();
    // g_pS5Sema   = xSemaphoreCreateMutex();
    // g_pS6Sema   = xSemaphoreCreateMutex();
    // g_pS7Sema   = xSemaphoreCreateMutex();

/* take all release semaphores to prevent immediate release upon scheduler start */
    // xSemaphoreTake(g_pS1Sema, portMAX_DELAY);
    // xSemaphoreTake(g_pS2Sema, portMAX_DELAY);
    // xSemaphoreTake(g_pS3Sema, portMAX_DELAY);
    // xSemaphoreTake(g_pS4Sema, portMAX_DELAY);
    // xSemaphoreTake(g_pS5Sema, portMAX_DELAY);
    // xSemaphoreTake(g_pS6Sema, portMAX_DELAY);
    // xSemaphoreTake(g_pS7Sema, portMAX_DELAY);


/* clear all release and abort flags */
    uint8_t i = 0;
    for(i = 0; i <= NUM_OF_SERVICES; i++) {
        abortSx[i]      = false;
        releaseSx[i]    = false;
        SxCnt[i]        = 0;
        g_pSxSema[i]    = xSemaphoreCreateMutex();

        xSemaphoreTake(g_pSxSema[i], portMAX_DELAY);
    }

/* create each service */
    // int ret = SequencerInit();
    // if(ret)
    // {
    //     UARTprintf("Failed to create Sequencer: %d\n", ret);
    //     return -1;
    // }

    int ret = Service1Init();
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
//
//    ret = Service3Init();
//    if(ret)
//    {
//        UARTprintf("Failed to create Service 3: %d\n", ret);
//        return -1;
//    }
//
//    ret = Service4Init();
//    if(ret)
//    {
//        UARTprintf("Failed to create Service 4: %d\n", ret);
//        return -1;
//    }
//
//    ret = Service5Init();
//    if(ret)
//    {
//        UARTprintf("Failed to create Service 5: %d\n", ret);
//        return -1;
//    }
//
//    ret = Service6Init();
//    if(ret)
//    {
//        UARTprintf("Failed to create Service 6: %d\n", ret);
//        return -1;
//    }
//
//    ret = Service7Init();
//    if(ret)
//    {
//        UARTprintf("Failed to create Service 7: %d\n", ret);
//        return -1;
//    }

    // Enable the timers.
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    while(1){
    // In case the scheduler returns for some reason, loop
    }
}
