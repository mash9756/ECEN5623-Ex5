#ifndef __SERVICES_H__
#define __SERVICES_H__

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Total services, sequencer + 7 services */
#define NUM_OF_SERVICES     (8)
#define SERVICE_STACK_SIZE  (128)         // Stack size in words
#define QUEUE_ITEM_SIZE     (sizeof(portTickType))
#define QUEUE_SIZE          (5)

typedef enum {
    SEQ = 0,
    S1,
    S2,
    S3,
    S4,
    S5,
    S6,
    S7 = 7
} Services_t;

uint32_t Service1Init(void);
uint32_t Service2Init(void);
uint32_t Service3Init(void);
uint32_t Service4Init(void);
uint32_t Service5Init(void);
uint32_t Service6Init(void);
uint32_t Service7Init(void);

bool abortSx[NUM_OF_SERVICES];
bool releaseSx[NUM_OF_SERVICES];
uint32_t SxCnt[NUM_OF_SERVICES];

xSemaphoreHandle g_pSxSema[NUM_OF_SERVICES];
// xSemaphoreHandle g_pS2Sema;
// xSemaphoreHandle g_pS3Sema;
// xSemaphoreHandle g_pS4Sema;
// xSemaphoreHandle g_pS5Sema;
// xSemaphoreHandle g_pS6Sema;
// xSemaphoreHandle g_pS7Sema;

xQueueHandle g_pS1Queue;
xQueueHandle g_pS2Queue;
xQueueHandle g_pS3Queue;
xQueueHandle g_pS4Queue;
xQueueHandle g_pS5Queue;
xQueueHandle g_pS6Queue;
xQueueHandle g_pS7Queue;

#endif // __SERVICES_H__
