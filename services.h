/**
 *  @name   services.h
 *  @brief  S1 - S7 service definitions header
 * 
 *  @author Mark Sherman
 * 
 * 
// Sequencer = RT_MAX	@ 30 Hz
// Servcie_1 = RT_MAX-1	@ 3 Hz      10
// Service_2 = RT_MAX-2	@ 1 Hz      30
// Service_3 = RT_MAX-3	@ 0.5 Hz    60
// Service_4 = RT_MAX-2	@ 1 Hz      30
// Service_5 = RT_MAX-3	@ 0.5 Hz    60
// Service_6 = RT_MAX-2	@ 1 Hz      30
// Service_7 = RT_MIN	0.1 Hz      300
*/

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

#define RUNTIME_S   (30)

#define SEQ_HZ  (30)
#define S1_HZ   (3)
#define S2_HZ   (1)
#define S3_HZ   (0.5)
#define S4_HZ   (1)
#define S5_HZ   (0.5)
#define S6_HZ   (1)
#define S7_HZ   (0.1)

/* compiler doesn't like using theses are array sizes */
// #define SEQ_REL_CNT (RUNTIME_S * SEQ_HZ)
// #define S1_REL_CNT  (RUNTIME_S * S1_HZ)
// #define S2_REL_CNT  (RUNTIME_S * S2_HZ)
// #define S3_REL_CNT  (RUNTIME_S * S3_HZ)
// #define S4_REL_CNT  (RUNTIME_S * S4_HZ)
// #define S5_REL_CNT  (RUNTIME_S * S5_HZ)
// #define S6_REL_CNT  (RUNTIME_S * S6_HZ)
// #define S7_REL_CNT  (RUNTIME_S * S7_HZ)

#define SEQ_REL_CNT (900)
#define S1_REL_CNT  (100)
#define S2_REL_CNT  (35)
#define S3_REL_CNT  (20)
#define S4_REL_CNT  (35)
#define S5_REL_CNT  (20)
#define S6_REL_CNT  (25)
#define S7_REL_CNT  (5)

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

uint32_t Service1Init(void);
uint32_t Service2Init(void);
uint32_t Service3Init(void);
uint32_t Service4Init(void);
uint32_t Service5Init(void);
uint32_t Service6Init(void);
uint32_t Service7Init(void);

#endif // __SERVICES_H__
