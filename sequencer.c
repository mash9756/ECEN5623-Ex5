/**
 * 
 *  @name   sequencer.c
 *  @brief  service sequencer definition
 * 
 *  @author Mark Sherman
*/

#include "sequencer.h"
#include "services.h"

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle g_pSx[NUM_OF_SERVICES];
extern bool abortSx[NUM_OF_SERVICES];
extern bool releaseSx[NUM_OF_SERVICES];
extern uint32_t SxCnt[NUM_OF_SERVICES];

static void Sequencer(void *threadp)
{
    uint8_t i = S1;    
    while(!abortSx[SEQ]) {
        xSemaphoreTake(g_pSx[SEQ], portMAX_DELAY);

        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\nSequencer Release Received from ISR!");
        xSemaphoreGive(g_pUARTSemaphore);

        for(; i < NUM_OF_SERVICES; i++) {
            if(releaseSx[i]) {
                xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
                UARTprintf("\nS%d Release Received from ISR!", i);
                xSemaphoreGive(g_pUARTSemaphore);
                xSemaphoreGive(g_pSx[i]);
                releaseSx[i] = false;
            }
        }
        i = S1;
        SxCnt[SEQ]++;
        if (SxCnt[SEQ] >= 1000) {
            abortSx[SEQ] = true;
        }
    }

    for (i = 0; i < NUM_OF_SERVICES; i++) {
        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\nS%d Release Count: %d",i , SxCnt[i]);
        xSemaphoreGive(g_pUARTSemaphore);
    }
}

uint32_t SequencerInit(void) {
    UARTprintf("Starting Sequencer...");

    abortSx[SEQ]    = false;
    releaseSx[SEQ]  = false;
    SxCnt[SEQ]      = 0;
    g_pSx[SEQ]      = xSemaphoreCreateMutex();
    xSemaphoreTake(g_pSx[SEQ], portMAX_DELAY);

    if(xTaskCreate(Sequencer, (signed portCHAR *)"Sequencer",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_SEQ, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}
