/**
 * 
 * 
 * 
*/

#include "sequencer.h"
#include "services.h"

xQueueHandle g_pSeqQueue;

static void Sequencer(void *threadp)
{
    while(!abortSx[SEQ]) {
        uint8_t i = S1;
        for(; i <= NUM_OF_SERVICES; i++) {
            if(releaseSx[i]) {
                xSemaphoreGive(g_pSxSema[i]);
                releaseSx[i] = false;
            }
        }

        SxCnt[SEQ]++;
        if (SxCnt[SEQ] > 1000) {
            abortSx[SEQ] = true;
        }
    }
}

uint32_t SequencerInit(void) {
    UARTprintf("Starting Sequencer...");
    //g_pSeqQueue = xQueueCreate(QUEUE_SIZE, QUEUE_ITEM_SIZE);

    if(xTaskCreate(Sequencer, (signed portCHAR *)"Sequencer",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_SEQ, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}
