/*
 * fib.c
 *
 *  Created on: Mar 28, 2024
 *      Author: marks
 */


#include <stdint.h>
#include "fib.h"

void fib_test(uint32_t seqCnt, uint32_t iterCnt) {
    uint32_t idx    = 0;
    uint32_t jdx    = 0;
    uint32_t fib    = 0;
    uint32_t fib0   = 0;
    uint32_t fib1   = 1;

    for(idx = 0; idx < iterCnt; idx++) {
        fib = fib0 + fib1;
        while(jdx < seqCnt) {
            fib0  = fib1;
            fib1  = fib;
            fib   = fib0 + fib1;
            jdx++;
        }
    }
}
