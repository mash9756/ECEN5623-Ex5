/**
 * 
 * 
 * 
*/

#include "services.h"

// xQueueHandle g_pS1Queue;
// xQueueHandle g_pS2Queue;
// xQueueHandle g_pS3Queue;
// xQueueHandle g_pS4Queue;
// xQueueHandle g_pS5Queue;
// xQueueHandle g_pS6Queue;
// xQueueHandle g_pS7Queue;

xSemaphoreHandle g_pUARTSemaphore;

static void Service1(void *pvParameters) {
    portTickType start;
    portTickType stop;
    //portTickType message;

    while(!abortSx[S1]) {
        xSemaphoreTake(g_pSxSema[S1], portMAX_DELAY);

        // if(xQueueReceive(g_pS1Queue, &message, 0) != pdPASS) {
        //     xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        //     UARTprintf("\nS1 ISR Message Error\n");
        //     xSemaphoreGive(g_pUARTSemaphore);
        // }
        //
        // xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        // UARTprintf("\nS1 Released, ISR time: %d", message);
        // xSemaphoreGive(g_pUARTSemaphore);

        start = xTaskGetTickCount();
        SxCnt[S1]++;
        stop = xTaskGetTickCount();

        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\nS1 completed in %dms | ", (stop - start));
        UARTprintf("Total Time: %dms\n",  stop);
        xSemaphoreGive(g_pUARTSemaphore);
    }
}

static void Service2(void *pvParameters) {
    portTickType start;
    portTickType stop;
    //portTickType message;

    while(!abortSx[S2]) {
        xSemaphoreTake(g_pSxSema[S2], portMAX_DELAY);

        // if(xQueueReceive(g_pS2Queue, &message, 0) != pdPASS) {
        //     xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        //     UARTprintf("\nS2 ISR Message Error\n");
        //     xSemaphoreGive(g_pUARTSemaphore);
        // }
        //
        // xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        // UARTprintf("\nS2 Released, ISR time: %d", message);
        // xSemaphoreGive(g_pUARTSemaphore);

        start = xTaskGetTickCount();
        SxCnt[S2]++;
        stop = xTaskGetTickCount();

        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\nS2 completed in %dms | ", (stop - start));
        UARTprintf("Total Time: %dms\n",  stop);
        xSemaphoreGive(g_pUARTSemaphore);
    }
}

static void Service3(void *pvParameters) {
    portTickType start;
    portTickType stop;
    //portTickType message;

    while(!abortSx[S3]) {
        xSemaphoreTake(g_pSxSema[S3], portMAX_DELAY);

        // if(xQueueReceive(g_pS3Queue, &message, 0) != pdPASS) {
        //     xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        //     UARTprintf("\nS3 ISR Message Error\n");
        //     xSemaphoreGive(g_pUARTSemaphore);
        // }
        //
        // xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        // UARTprintf("\nS3 Released, ISR time: %d", message);
        // xSemaphoreGive(g_pUARTSemaphore);

        start = xTaskGetTickCount();
        SxCnt[S3]++;
        stop = xTaskGetTickCount();

        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\nS3 completed in %dms | ", (stop - start));
        UARTprintf("Total Time: %dms\n",  stop);
        xSemaphoreGive(g_pUARTSemaphore);
    }
}

static void Service4(void *pvParameters) {
    portTickType start;
    portTickType stop;
    //portTickType message;

    while(!abortSx[S4]) {
        xSemaphoreTake(g_pSxSema[S4], portMAX_DELAY);

        // if(xQueueReceive(g_pS4Queue, &message, 0) != pdPASS) {
        //     xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        //     UARTprintf("\nS4 ISR Message Error\n");
        //     xSemaphoreGive(g_pUARTSemaphore);
        // }
        //
        // xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        // UARTprintf("\nS4 Released, ISR time: %d", message);
        // xSemaphoreGive(g_pUARTSemaphore);

        start = xTaskGetTickCount();
        SxCnt[S4]++;
        stop = xTaskGetTickCount();

        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\nS4 completed in %dms | ", (stop - start));
        UARTprintf("Total Time: %dms\n",  stop);
        xSemaphoreGive(g_pUARTSemaphore);
    }
}

static void Service5(void *pvParameters) {
    portTickType start;
    portTickType stop;
    //portTickType message;

    while(!abortSx[S5]) {
        xSemaphoreTake(g_pSxSema[S5], portMAX_DELAY);

        // if(xQueueReceive(g_pS5Queue, &message, 0) != pdPASS) {
        //     xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        //     UARTprintf("\nS5 ISR Message Error\n");
        //     xSemaphoreGive(g_pUARTSemaphore);
        // }
        //
        // xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        // UARTprintf("\nS5 Released, ISR time: %d", message);
        // xSemaphoreGive(g_pUARTSemaphore);

        start = xTaskGetTickCount();
        SxCnt[S5]++;
        stop = xTaskGetTickCount();

        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\nS5 completed in %dms | ", (stop - start));
        UARTprintf("Total Time: %dms\n",  stop);
        xSemaphoreGive(g_pUARTSemaphore);
    }
}

static void Service6(void *pvParameters) {
    portTickType start;
    portTickType stop;
    //portTickType message;

    while(!abortSx[S6]) {
        xSemaphoreTake(g_pSxSema[S6], portMAX_DELAY);

        // if(xQueueReceive(g_pS6Queue, &message, 0) != pdPASS) {
        //     xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        //     UARTprintf("\nS6 ISR Message Error\n");
        //     xSemaphoreGive(g_pUARTSemaphore);
        // }
        //
        // xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        // UARTprintf("\nS6 Released, ISR time: %d", message);
        // xSemaphoreGive(g_pUARTSemaphore);

        start = xTaskGetTickCount();
        SxCnt[S6]++;
        stop = xTaskGetTickCount();

        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\nS6 completed in %dms | ", (stop - start));
        UARTprintf("Total Time: %dms\n",  stop);
        xSemaphoreGive(g_pUARTSemaphore);
    }
}

static void Service7(void *pvParameters) {
    portTickType start;
    portTickType stop;
    //portTickType message;

    while(!abortSx[S7]) {
        xSemaphoreTake(g_pSxSema[S7], portMAX_DELAY);

        // if(xQueueReceive(g_pS7Queue, &message, 0) != pdPASS) {
        //     xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        //     UARTprintf("\nS7 ISR Message Error\n");
        //     xSemaphoreGive(g_pUARTSemaphore);
        // }
        //
        // xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        // UARTprintf("\nS7 Released, ISR time: %d", message);
        // xSemaphoreGive(g_pUARTSemaphore);

        start = xTaskGetTickCount();
        SxCnt[S7]++;
        stop = xTaskGetTickCount();

        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\nS7 completed in %dms | ", (stop - start));
        UARTprintf("Total Time: %dms\n",  stop);
        xSemaphoreGive(g_pUARTSemaphore);
    }
}

uint32_t Service1Init(void) {
    UARTprintf("Starting Service 1...");
    //g_pS1Queue = xQueueCreate(QUEUE_SIZE, QUEUE_ITEM_SIZE);

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
    //g_pS2Queue = xQueueCreate(QUEUE_SIZE, QUEUE_ITEM_SIZE);

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
    //g_pS3Queue = xQueueCreate(QUEUE_SIZE, QUEUE_ITEM_SIZE);

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
    //g_pS4Queue = xQueueCreate(QUEUE_SIZE, QUEUE_ITEM_SIZE);

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
    //g_pS5Queue = xQueueCreate(QUEUE_SIZE, QUEUE_ITEM_SIZE);

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
    //g_pS6Queue = xQueueCreate(QUEUE_SIZE, QUEUE_ITEM_SIZE);

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
    //g_pS7Queue = xQueueCreate(QUEUE_SIZE, QUEUE_ITEM_SIZE);

    if(xTaskCreate(Service7, (signed portCHAR *)"S7",
                   SERVICE_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_S7, NULL) != pdTRUE)
    {
        return(1);
    }

    UARTprintf("Done!\n");
    return(0);
}
