/*
 * driver.h
 *
 *  Created on: 5 giu 2024
 *      Author: albanese
 */

#ifndef DRIVERS_DRIVER_H_
#define DRIVERS_DRIVER_H_

#ifdef __cplusplus
extern "C"
{
#endif

//FUNCTION USED FOR GETTING OR SETTING VALUE
int ValueGet(unsigned int Slot, unsigned int Channel);
void ValueSet(unsigned int Slot, unsigned int Channel, int value);

#ifdef __cplusplus
}
#endif


#endif /* DRIVERS_DRIVER_H_ */
