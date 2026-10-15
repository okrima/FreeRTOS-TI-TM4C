/*
 * driver_TimerPwm.h
 *
 *  Created on: 04.06.2024
 *      Author: Mirko Albanese
 */

#ifndef __driver_TimerPwm__
#define __driver_TimerPwm__

//#include "utility_TM4C129X_.h"

#define NUM_TIMER_PWM   2

typedef struct
{
    uint32_t ui32Base;
    uint32_t ui32Timer;
    int32_t value;
}tTimerPwmStructTab;

extern tTimerPwmStructTab TimerPwm[NUM_TIMER_PWM];

#ifdef __cplusplus
extern "C"
{
#endif

int32_t getTimerPwmGetValue(tTimerPwmStructTab * TimerPwmTemp);
void setTimerPwmGetValue(tTimerPwmStructTab * TimerPwmTemp, int32_t value);

void TimerPwmInit(tTimerPwmStructTab *TimerPwmtemp, uint32_t Channel);
void driverTimerPwm();

#ifdef __cplusplus
}
#endif

#endif  //__driver_TimerPwm__
