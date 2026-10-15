/*
 * Timer.h
 *
 *  Created on: 12 giu 2024
 *      Author: albanese
 */

#ifndef SRC_APPLICATION_TIMERHDL_H_
#define SRC_APPLICATION_TIMERHDL_H_

//Global timer
extern unsigned long GlobalTimer;

extern unsigned char interrupt1msDisabled;

#ifdef __cplusplus
extern "C"
{
#endif

void TimerHdl();

#ifdef __cplusplus
}
#endif

#endif /* SRC_APPLICATION_TIMERHDL_H_ */
