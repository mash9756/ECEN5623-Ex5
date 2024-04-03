/**
 *  @name   services.h
 *  @brief  S1 - S7 service definitions header
 * 
 *  @author Mark Sherman
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