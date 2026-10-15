/*
 * multdiv.h
 *
 *  Created on: 30 mag 2024
 *      Author: albanese
 */

#ifndef DRIVERS_MULTDIV_H_
#define DRIVERS_MULTDIV_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

uint32_t   muldivu(uint32_t wert,uint32_t mul,uint32_t divisor);
int32_t muldiv(int32_t p1,int32_t p2,int32_t p3);

#ifdef __cplusplus
}
#endif



#endif /* DRIVERS_MULTDIV_H_ */
