/**
 *  @name   services.c
 *  @brief  S1 - S7 service definitions
 * 
 *  @author Mark Sherman
*/

#include "services.h"
#include "fib.h"

extern xSemaphoreHandle g_pUARTSemaphore;
extern xSemaphoreHandle g_pSx[NUM_OF_SERVICES];
extern bool abortSx[NUM_OF_SERVICES];
extern bool releaseSx[NUM_OF_SERVICES];
extern uint32_t SxCnt[NUM_OF_SERVICES];

/* not super clean but didn't want to waste too much time making this prettier */
portTickType service1_executionTime[S1_REL_CNT];
portTickType service2_executionTime[S2_REL_CNT];
portTickType service3_executionTime[S3_REL_CNT];
portTickType service4_executionTime[S4_REL_CNT];
portTickType service5_executionTime[S5_REL_CNT];
portTickType service6_executionTime[S6_REL_CNT];
portTickType service7_executionTime[S7_REL_CNT];

static void Service1(void *pvParameters) {
    portTickType start;
    portTickType stop;

    while(!abortSx[S1]) {
        xSemaphoreTake(g_pSx[S1], portMAX_DELAY);

        start = xTaskGetTickCount();
        fib_test(FIB10MS_ITERATIONS);   
        stop = xTaskGetTickCount();

        service1_executionTime[SxCnt[S1]] = stop - start;
        SxCnt[S1]++;
    }
}

static void Service2(void *pvParameters) {
    portTickType start;
    portTickType stop;

    while(!abortSx[S2]) {
        xSemaphoreTake(g_pSx[S2], portMAX_DELAY);

        start = xTaskGetTickCount();
        fib_test(FIB10MS_ITERATIONS);
        stop = xTaskGetTickCount();

        service2_executionTime[SxCnt[S2]] = stop - start;
        SxCnt[S2]++;
    }
}

static void Service3(void *pvParameters) {
    portTickType start;
    portTickType stop;

    while(!abortSx[S3]) {
        xSemaphoreTake(g_pSx[S3], portMAX_DELAY);

        start = xTaskGetTickCount();
        fib_test(FIB10MS_ITERATIONS);
        stop = xTaskGetTickCount();

        service3_executionTime[SxCnt[S3]] = stop - start;
        SxCnt[S3]++;
    }
}

static void Service4(void *pvParameters) {
    portTickType start;
    portTickType stop;

    while(!abortSx[S4]) {
        xSemaphoreTake(g_pSx[S4], portMAX_DELAY);

        start = xTaskGetTickCount();
        fib_test(FIB10MS_ITERATIONS);
        stop = xTaskGetTickCount();

        service4_executionTime[SxCnt[S4]] = stop - start;
        SxCnt[S4]++;
    }
}

static void Service5(void *pvParameters) {
    portTickType start;
    portTickType stop;

    while(!abortSx[S5]) {
        xSemaphoreTake(g_pSx[S5], portMAX_DELAY);

        start = xTaskGetTickCount();
        fib_test(FIB10MS_ITERATIONS);
        stop = xTaskGetTickCount();

        service5_executionTime[SxCnt[S5]] = stop - start;
        SxCnt[S5]++;
    }
}

static void Service6(void *pvParameters) {
    portTickType start;
    portTickType stop;

    while(!abortSx[S6]) {
        xSemaphoreTake(g_pSx[S6], portMAX_DELAY);

        start = xTaskGetTickCount();
        fib_test(FIB10MS_ITERATIONS);
        stop = xTaskGetTickCount();

        service6_executionTime[SxCnt[S6]] = stop - start;
        SxCnt[S6]++;
    }
}

static void Service7(void *pvParameters) {
    portTickType start;
    portTickType stop;

    while(!abortSx[S7]) {
        xSemaphoreTake(g_pSx[S7], portMAX_DELAY);

        start = xTaskGetTickCount();
        fib_test(FIB10MS_ITERATIONS);
        stop = xTaskGetTickCount();

        service7_executionTime[SxCnt[S7]] = stop - start;
        SxCnt[S7]++;
    }
}

uint32_t Service1Init(void) {
    UARTprintf("Starting Service 1...");

    if(xTaskCreate(Service1, (signed portCHAR *)"S1",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_S1, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}

uint32_t Service2Init(void) {
    UARTprintf("Starting Service 2...");

    if(xTaskCreate(Service2, (signed portCHAR *)"S2",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_S2, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}

uint32_t Service3Init(void) {
    UARTprintf("Starting Service 3...");

    if(xTaskCreate(Service3, (signed portCHAR *)"S3",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_S3, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}

uint32_t Service4Init(void) {
    UARTprintf("Starting Service 4...");

    if(xTaskCreate(Service4, (signed portCHAR *)"S4",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_S4, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}

uint32_t Service5Init(void) {
    UARTprintf("Starting Service 5...");

    if(xTaskCreate(Service5, (signed portCHAR *)"S5",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_S5, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}

uint32_t Service6Init(void) {
    UARTprintf("Starting Service 6...");

    if(xTaskCreate(Service6, (signed portCHAR *)"S6",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_S6, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}

uint32_t Service7Init(void) {
    UARTprintf("Starting Service 7...");

    if(xTaskCreate(Service7, (signed portCHAR *)"S7",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_S7, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}
