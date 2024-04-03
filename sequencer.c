/**
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

/* not super clean but didn't want to waste too much time making this prettier */
extern portTickType service1_executionTime[S1_REL_CNT];
extern portTickType service2_executionTime[S2_REL_CNT];
extern portTickType service3_executionTime[S3_REL_CNT];
extern portTickType service4_executionTime[S4_REL_CNT];
extern portTickType service5_executionTime[S5_REL_CNT];
extern portTickType service6_executionTime[S6_REL_CNT];
extern portTickType service7_executionTime[S7_REL_CNT];

portTickType WCET[NUM_OF_SERVICES];

/* not super clean but didn't want to waste too much time making this prettier */
void calcWCET() {
    uint8_t i = 0;
    
    for (; i < S1_REL_CNT; i++) {
        if(WCET[S1] < service1_executionTime[i]){
            WCET[S1] = service1_executionTime[i];
        }
    }
    for (i = 0; i < S2_REL_CNT; i++) {
        if(WCET[S2] < service2_executionTime[i]){
            WCET[S2] = service2_executionTime[i];
        }
    }
    for (i = 0; i < S3_REL_CNT; i++) {
        if(WCET[S3] < service3_executionTime[i]){
            WCET[S3] = service3_executionTime[i];
        }
    }
    for (i = 0; i < S4_REL_CNT; i++) {
        if(WCET[S4] < service4_executionTime[i]){
            WCET[S4] = service4_executionTime[i];
        }
    }
    for (i = 0; i < S5_REL_CNT; i++) {
        if(WCET[S5] < service5_executionTime[i]){
            WCET[S5] = service5_executionTime[i];
        }
    }
    for (i = 0; i < S6_REL_CNT; i++) {
        if(WCET[S6] < service6_executionTime[i]){
            WCET[S6] = service6_executionTime[i];
        }
    }
    for (i = 0; i < S7_REL_CNT; i++) {
        if(WCET[S7] < service7_executionTime[i]){
            WCET[S7] = service7_executionTime[i];
        }
    }    
}

static void Sequencer(void *threadp)
{
    uint8_t i = S1;
    xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
    UARTprintf("Execution Status:\n");
    xSemaphoreGive(g_pUARTSemaphore);    

    while(!abortSx[SEQ]) {
        xSemaphoreTake(g_pSx[SEQ], portMAX_DELAY);
        
        if(SxCnt[SEQ] % 30){
            xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
            UARTprintf("-");
            xSemaphoreGive(g_pUARTSemaphore);
        }

        for(; i < NUM_OF_SERVICES; i++) {
            if(releaseSx[i]) {
                xSemaphoreGive(g_pSx[i]);
                releaseSx[i] = false;
            }
        }
        i = S1;
        SxCnt[SEQ]++;
        if (SxCnt[SEQ] >= SEQ_REL_CNT) {
            abortSx[SEQ] = true;
        }
    }
    calcWCET();
    for (i = 0; i < NUM_OF_SERVICES; i++) {
        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\nS%d Release Count: %d | WCET: %dms",i , SxCnt[i], WCET[i]);
        xSemaphoreGive(g_pUARTSemaphore);
    }
}

uint32_t SequencerInit(void) {
    UARTprintf("Starting Sequencer...");

/* init sequencer variables */
    abortSx[SEQ]    = false;
    releaseSx[SEQ]  = false;
    SxCnt[SEQ]      = 0;
    g_pSx[SEQ]      = xSemaphoreCreateMutex();
    xSemaphoreTake(g_pSx[SEQ], portMAX_DELAY);

/* init WCETs */
    uint8_t i = 0;
    for (i = 0; i < NUM_OF_SERVICES; i++) {
        WCET[i] = 0;
    }

    if(xTaskCreate(Sequencer, (signed portCHAR *)"Sequencer",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_SEQ, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}
