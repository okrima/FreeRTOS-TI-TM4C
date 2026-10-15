/*
 * driver_Dig.h
 *
 *  Created on: 05.06.2024
 *      Author: Mirko Albanese
 */
/*
 *
*/

#ifndef DRIVERS___TM4C129X___DRIVER_DIG_H_
#define DRIVERS___TM4C129X___DRIVER_DIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

void setDigitalOutputValue(uint32_t Channel, int32_t value);
int32_t getDigitalOutputValue(uint32_t Channel);

int32_t getDigitalInputValue(uint32_t Channel);
void setDigitalInputValue(uint32_t Channel, int32_t value);

void driver_Dig_init();

void HwDigTicker(void *pvParameters);

#ifdef __cplusplus
}
#endif



#endif /* DRIVERS___TM4C129X___DRIVER_DIG_H_ */
