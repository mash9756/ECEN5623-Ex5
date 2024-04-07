/**
 *  @name   services.c
 *  @brief  S1 - S7 service definitions
 * 
 *  @author Mark Sherman
*/

#include "services.h"
#include "fib.h"
#include "stdio.h"

/* global static instance of array of all services */
static Services_t services[NUM_OF_SERVICES];

/* array of priorities for service creation */
const uint32_t SERVICE_PRIORITIES[NUM_OF_SERVICES] = {
    PRIORITY_SEQ,
    PRIORITY_S1,
    PRIORITY_S2,
    PRIORITY_S3,
    PRIORITY_S4,
    PRIORITY_S5,
    PRIORITY_S6,
    PRIORITY_S7
};

const uint32_t EXPECTED_RELEASE_COUNTS[NUM_OF_SERVICES] = {
    SEQ_REL_CNT,
    S1_REL_CNT,
    S2_REL_CNT,
    S3_REL_CNT,
    S4_REL_CNT,
    S5_REL_CNT,
    S6_REL_CNT,
    S7_REL_CNT
};

/* service name getter for service creation */
char *cGetServiceName(SNames_t id){
    switch (id)
    {
        case SEQ:
            return "SEQ";
        case S1:
            return "S1";
        case S2:
            return "S2";
        case S3:
            return "S3";
        case S4:
            return "S4";
        case S5:
            return "S5";
        case S6:
            return "S6";
        case S7:
            return "S7";
        default:
            return "???";
    }
}

/* arrays for each service since they have different release periods */
portTickType service1_executionTime[S1_REL_CNT];
portTickType service2_executionTime[S2_REL_CNT];
portTickType service3_executionTime[S3_REL_CNT];
portTickType service4_executionTime[S4_REL_CNT];
portTickType service5_executionTime[S5_REL_CNT];
portTickType service6_executionTime[S6_REL_CNT];
portTickType service7_executionTime[S7_REL_CNT];

/* array of pointers to ex time arrays for assignment to each service */
portTickType *xExecutionTimes[NUM_OF_SERVICES] = {
    NULL,
    service1_executionTime,
    service2_executionTime,
    service3_executionTime,
    service4_executionTime,
    service5_executionTime,
    service6_executionTime,
    service7_executionTime
};

/* getter for a reference to static array of services  */
Services_t *xGetServices() {
    return services;
}

/* getter for service prioritiess */
uint32_t uiGetRelCnt(SNames_t id) {
    return EXPECTED_RELEASE_COUNTS[(uint32_t)id];
}

/* getter for service prioritiess */
uint32_t uiGetPriority(SNames_t id) {
    return SERVICE_PRIORITIES[(uint32_t)id];
}

/* service function with synthetic fibonacci load */
static void vServFunc(void *pvParameters) {
    uint32_t servID = 0;
    servID = (uint32_t)pvParameters;

    portTickType start;
    portTickType stop;

    while(!services[servID].abort) {
        xSemaphoreTake(services[servID].sem, portMAX_DELAY);

        start = xTaskGetTickCount();
        fib_test(FIB3000HZ_ITERATIONS);
        stop = xTaskGetTickCount();

        services[servID].exTimes[services[servID].rel_cnt] = stop - start;
        services[servID].rel_cnt++;
    }
}

/* initializer for all services */
uint32_t uiInitServices(void) {
    uint8_t i   = S1;
    int ret     = 0;

    for(; i < NUM_OF_SERVICES ; i++) {
        UARTprintf("Starting Service %d...", i);

        services[i].id      = (SNames_t)i;
        services[i].prio    = uiGetPriority((SNames_t)i);
        services[i].abort   = false;
        services[i].release = false;
        services[i].rel_cnt = 0;
        services[i].WCET    = 0;
        services[i].minET   = 0xff;

        services[i].exp_rel_cnt = uiGetRelCnt((SNames_t)i);
        services[i].exTimes = xExecutionTimes[i];

        vSemaphoreCreateBinary(services[i].sem);
        xSemaphoreTake(services[i].sem, portMAX_DELAY);

        ret = xTaskCreate(  vServFunc, 
                            (signed portCHAR *)cGetServiceName(services[i].id),
                            SERVICE_STACK_SIZE, 
                            (void *)services[i].id, 
                            tskIDLE_PRIORITY + services[i].prio, 
                            NULL);
        if(ret != pdTRUE) {
            return(1);
        }

        UARTprintf("Done!\n");
    }
    return(0);
}
