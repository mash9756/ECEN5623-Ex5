/**
 *  @name   sequencer.c
 *  @brief  service sequencer definition
 * 
 *  @author Mark Sherman
*/

#include "sequencer.h"
#include "services.h"

void calcWCET() {
    uint8_t i = S1;
    uint8_t j = 0;

    Services_t *services = xGetServices();

    for (; i < NUM_OF_SERVICES; i++) {
        for (j = 0; j < services[i].exp_rel_cnt ; j++)
        {
            if(services[i].WCET < services[i].exTimes[j]){
                services[i].WCET = services[i].exTimes[j];
            }
             if(services[i].minET > services[i].exTimes[j]){
                services[i].minET = services[i].exTimes[j];
            }
        }
    }
}

static void vSequencer(void *threadp)
{
    Services_t *services = xGetServices();
    uint8_t i = S1;
    UARTprintf("\nExecution Status:\n"); 
    
    while(!services[SEQ].abort) {
        xSemaphoreTake(services[SEQ].sem, portMAX_DELAY);
       
        if(services[SEQ].rel_cnt % 10 == 0){
            UARTprintf("-");
        }
    
        for(i = S1; i < NUM_OF_SERVICES; i++) {
            if(services[i].release) {
                xSemaphoreGive(services[i].sem);
                services[i].release = false;
            }
        }
        
        services[SEQ].rel_cnt++;
        if (services[SEQ].rel_cnt >= SEQ_REL_CNT) {
            ROM_IntDisable(INT_TIMER0A);
            for (i = SEQ; i < NUM_OF_SERVICES; i++) {
                services[i].abort = true;
            }
        }
    }

    calcWCET();
    for (i = S1; i < NUM_OF_SERVICES; i++) {
        UARTprintf("\nS%d Release Count: %d| WCET: %dms | minET: %dms", i, services[i].rel_cnt, services[i].WCET, services[i].minET);
    }
}

uint32_t SequencerInit(void) {
    UARTprintf("\n\nStarting Sequencer...");
    int ret = 0;
    Services_t *services = xGetServices();

/* init sequencer variables */
    services[SEQ].id      = SEQ;
    services[SEQ].prio    = uiGetPriority((SNames_t)SEQ);
    services[SEQ].abort   = false;
    services[SEQ].release = false;
    services[SEQ].rel_cnt = 0;
    services[SEQ].WCET    = 0;
    services[SEQ].minET   = 0xff;

    services[SEQ].exp_rel_cnt = uiGetRelCnt(SEQ);
    services[SEQ].exTimes = NULL;

    vSemaphoreCreateBinary(services[SEQ].sem);
    xSemaphoreTake(services[SEQ].sem, portMAX_DELAY);

    ret = xTaskCreate(  vSequencer, 
                        (signed portCHAR *)cGetServiceName(SEQ),
                        SERVICE_STACK_SIZE, 
                        NULL, 
                        tskIDLE_PRIORITY + services[SEQ].prio, 
                        NULL);
    if(ret != pdTRUE) {
        return(1);
    }

    UARTprintf("Done!");
    return(0);
}
