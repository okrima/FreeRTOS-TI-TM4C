/*
 * driver.cpp
 *
 *  Created on: 5 giu 2024
 *      Author: albanese
 */

#include "stdint.h"

#include "driver_ADC.h"
#include "driver_cSsi.h"
#include "driver_Dig.h"
#include "driver_PWM.h"
#include "driver_TimerCounter.h"
#include "driver_TimerPWM.h"
#include "driver_ZeroCrossPhaseCut.h"
#include "S13_presence.h"
#include "utility_TM4C129X_.h"

#include "driver.h"


//////////////////////////////////////////////////////////////////////////////////////////////
// connections from application to hardware class
//////////////////////////////////////////////////////////////////////////////////////////////
//int32_t ValueGet(uint32_t Slot, uint32_t Channel)                           { return driverGetValue(Slot, Channel); }
//void ValueSet(uint32_t Slot, uint32_t Channel, int32_t value)               { return driverSetValue(Slot, Channel, value); }


