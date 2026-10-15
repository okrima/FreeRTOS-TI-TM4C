/*
 * S13_precence.h
 *
 *  Created on: 05.06.2024
 *      Author: Mirko Albanese
 */

#ifndef __S13_presence__
#define __S13_presence__

#include "pindef__TM4C129X__.h"

#ifdef __cplusplus
extern "C"
{
#endif

void S13_Presence_Init(void);
unsigned int S13_presence(void);

#ifdef __cplusplus
}
#endif

#endif /*__S13_presence__ */
