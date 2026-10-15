/*
 * driver_ADC.h
 *
 *  Created on: 05.06.2024
 *      Author: Mirko Albanese
 */
/*
 *
*/

#ifndef driverTM4C129X_ADC_H_
#define driverTM4C129X_ADC_H_

#define NUMBER_OF_ADC   11

extern int32_t TickerPhase;

typedef struct
{
    uint32_t AverageCount;
    uint32_t AverageCommulation;
    uint32_t value;
}tAdcStructTab;

extern tAdcStructTab Adc[NUMBER_OF_ADC];

#ifdef __cplusplus
extern "C"
{
#endif

void setADCValue(tAdcStructTab* adc_temp);
void CommulateValue(tAdcStructTab* adc_temp, uint32_t AdcValue);

void HwADCTicker(void *pvParameters);

int32_t GetADCValue(uint32_t Channel);

void driverTM4C129X_ADCs_init();

#ifdef __cplusplus
}
#endif


#endif // driverTM4C129X_ADC_H_
