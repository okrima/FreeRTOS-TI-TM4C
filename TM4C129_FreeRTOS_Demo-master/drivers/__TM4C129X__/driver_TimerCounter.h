/*
 * driver_TimerCounter.h
 *
 *  Created on: 05.06.2024
 *      Author: Mirko Albanese
 */
/*
 *
*/

#ifndef __driver_TimerCounter__
#define __driver_TimerCounter__

#define NUMBER_OF_TIMER 3

typedef struct
{
    uint32_t TimerValueBase;
    uint16_t  CountTicker;
    uint16_t  CountLast;
    uint16_t  ImpPerSec;
    char CountInitialized;
    uint32_t value;
}tTimerStructTab;

extern tTimerStructTab Timer[NUMBER_OF_TIMER];

#ifdef __cplusplus
extern "C"
{
#endif

void driverTimerCounter(tTimerStructTab *Timer, uint16_t Channel);
void driverTimerCounterInit();

uint32_t getTimerValue(tTimerStructTab *Timer);
void setTimerValue(tTimerStructTab *Timer, uint32_t value);
void HwTimerTicker(tTimerStructTab *Timer);
void HwTimerTick(void *pvParameters);

void driverTimerCounterInit();

#ifdef __cplusplus
}
#endif

#endif //__driver_TimerCounter__
