/*
 * cSsiC2020.h
 *
 *  Created on: 29.05.2024
 *      Author: Mirko Albanese
 */

#ifndef CSSIC2020_H_
#define CSSIC2020_H_

#include "ssi.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wait_us(unsigned int us);

void expansionTransfer(char *dst,const char *src, uint32_t len);
void expansionSetSpeed(uint32_t Hz);

void driverExpansionInit(uint32_t Hz);

#ifdef __cplusplus
}
#endif

#endif /* CSSIC2020_H_ */
