/**
 *  @name   services.h
 *  @brief  S1 - S7 service header
 * 
 *  @author Mark Sherman
 * 
 * Sequencer = RT_MAX	@ 30 Hz
 * Servcie_1 = RT_MAX-1	@ 3 Hz      10
 * Service_2 = RT_MAX-2	@ 1 Hz      30
 * Service_3 = RT_MAX-3	@ 0.5 Hz    60
 * Service_4 = RT_MAX-2	@ 1 Hz      30
 * Service_5 = RT_MAX-3	@ 0.5 Hz    60
 * Service_6 = RT_MAX-2	@ 1 Hz      30
 * Service_7 = RT_MIN	0.1 Hz      300
*/

#ifndef __SERVICES_H__
#define __SERVICES_H__

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* define flag for 30Hz / 3000Hz */
#define SEQ_30HZ        (1)

/* Total services, sequencer + 7 services */
#define NUM_OF_SERVICES     (8)
#define SERVICE_STACK_SIZE  (128)         // Stack size in words

/* Calculated service release counts based on sequencer iterations */
#define SEQ_REL_CNT (900)
#define S1_REL_CNT  (SEQ_REL_CNT / 10)
#define S2_REL_CNT  (SEQ_REL_CNT / 30)
#define S3_REL_CNT  (SEQ_REL_CNT / 60)
#define S4_REL_CNT  (SEQ_REL_CNT / 30)
#define S5_REL_CNT  (SEQ_REL_CNT / 60)
#define S6_REL_CNT  (SEQ_REL_CNT / 30)
#define S7_REL_CNT  (SEQ_REL_CNT / 300)

typedef enum {
    SEQ = 0,
    S1,
    S2,
    S3,
    S4,
    S5,
    S6,
    S7 = 7
} SNames_t;

typedef struct 
{
    SNames_t id;
    uint32_t prio;
    bool release;
    bool abort;
    uint32_t rel_cnt;
    uint32_t exp_rel_cnt;
    portTickType *exTimes;
    portTickType WCET;
    portTickType minET;
    xSemaphoreHandle sem;
} Services_t;

Services_t *xGetServices();
char *cGetServiceName(SNames_t id);
uint32_t uiGetRelCnt(SNames_t id);
uint32_t uiGetPriority(SNames_t id);
uint32_t uiInitServices(void);

#endif // __SERVICES_H__
