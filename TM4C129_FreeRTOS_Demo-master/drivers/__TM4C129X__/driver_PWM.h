/*
 * driver_PWM.h
 *
 *  Created on: 05.06.2024
 *      Author: Mirko Albanese
 */
/*
 *
*/

#ifndef __driver_PWM__
#define __driver_PWM__

//#include "utility_TM4C129X_.h"

#define NUMER_OF_PWM_OUTPUT 3

typedef struct
{
    uint32_t PwmEnableBase;
    uint32_t PwmEnableBit;
    uint32_t PwmxCmpBase;
    uint32_t value;
}tPwmStructTab;

extern tPwmStructTab Pwm[NUMER_OF_PWM_OUTPUT];

#ifdef __cplusplus
extern "C"
{
#endif

void driverPWM(tPwmStructTab * Pwm, uint32_t Channel);
void setPwmValue(tPwmStructTab * Pwm, int32_t value);
uint32_t getPWMValue(tPwmStructTab * Pwm);

void init_driver_pwm();

#ifdef __cplusplus
}
#endif


#endif //__driver_PWM__
