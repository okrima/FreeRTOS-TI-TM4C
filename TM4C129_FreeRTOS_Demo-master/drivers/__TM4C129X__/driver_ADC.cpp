/*
 * driver_ADC.cpp
 *
 *  Created on: 06.06.2024
 *      Author: Mirko Albanese
 */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// driver for ADC
//
// ADC:  http://datasheet.digitronic.com/ARM/STELLARIS-TEXAS-INSTRUMENTS/TM4/TM4C1294_M4_1024Flash_256SRAM_ETH_MAC_PHY.pdf#page=1053
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdint.h"

#include "FreeRTOS.h"

#include "driver_ADC.h"
#include "utility_TM4C129X_.h"
#include "adc.h"
#include "hw_adc.h"
#include "hw_timer.h"
#include "hw_types.h"
#include "sysctl.h"
#include "pin_map.h"
#include "gpio.h"
#include "hw_memmap.h"
#include "multdiv.h"
#include "pwm.h"
#include "hw_pwm.h"
#include "stdlib.h"
#include "task.h"



#define MacADCProcessorTrigger(ui32AdcBase,ui32SequenceNum)     (HWREG((ui32AdcBase) + ADC_O_PSSI) = 1 << (ui32SequenceNum))
#define MacADCSeqBase(ui32AdcBase,ui32SequenceNum)              ((ui32AdcBase) + ADC_O_SSMUX0 + ((ADC_O_SSMUX1 - ADC_O_SSMUX0) * (ui32SequenceNum)))
#define MacADCFifoEmpty(ui32AdcBase,ui32SequenceNum)            ((HWREG(MacADCSeqBase(ui32AdcBase,ui32SequenceNum) + (ADC_O_SSFSTAT0 - ADC_O_SSMUX0)) & ADC_SSFSTAT0_EMPTY))
#define MacADCSequenceDataGet(ui32AdcBase,ui32SequenceNum)      (HWREG(MacADCSeqBase(ui32AdcBase,ui32SequenceNum) + (ADC_O_SSFIFO0 - ADC_O_SSMUX0)))

int32_t TickerPhase = 0;
int32_t AdcIndex = 0;

uint32_t AverageMs = 16;
//////////////////////////////////////////////////////////////////////////////////////////
// class for one ADC
//////////////////////////////////////////////////////////////////////////////////////////

tAdcStructTab Adc[NUMBER_OF_ADC] = {0};

void setADCValue(tAdcStructTab* adc_temp)
{
    adc_temp->value = adc_temp->AverageCommulation>>4;
}

void CommulateValue(tAdcStructTab* adc_temp, uint32_t AdcValue)
{
    //C2020Data.Adc[AdcIndex] = AdcValue;
    adc_temp->AverageCommulation+=AdcValue;
    if(++(adc_temp->AverageCount) >= AverageMs)
    {
        setADCValue(adc_temp);  // MOD MNS
        adc_temp->AverageCommulation=0;
        adc_temp->AverageCount=0;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// common class for all ADC's
//////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    uint32_t PeripheralEnablePort;
    uint32_t ADC_CTL_CHx;
    uint32_t ui32PortBase;
    uint32_t Pins;
}tAdcConfigTab;

// { MOD MNS

static const tAdcConfigTab AdcConfigTab[]=
//port activating    ,adc channel                                ,port base      ,pin of port
{{SYSCTL_PERIPH_GPIOE, ADC_CTL_CH1 |ADC_CTL_SHOLD_64             ,GPIO_PORTE_BASE,GPIO_PIN_2}                  // -35°/+90° PTC X41 T3
,{SYSCTL_PERIPH_GPIOE, ADC_CTL_CH2 |ADC_CTL_SHOLD_64             ,GPIO_PORTE_BASE,GPIO_PIN_1}                  // -35°/+90° PTC X43 T2
,{SYSCTL_PERIPH_GPIOK, ADC_CTL_CH16|ADC_CTL_SHOLD_64             ,GPIO_PORTK_BASE,GPIO_PIN_0}                  // -35°/+90° PTC X29 T1
,{SYSCTL_PERIPH_GPIOE, ADC_CTL_CH3 |ADC_CTL_SHOLD_64             ,GPIO_PORTE_BASE,GPIO_PIN_0}                  // -35°/+90° PTC X44 T4
,{SYSCTL_PERIPH_GPIOE, ADC_CTL_CH0 |ADC_CTL_SHOLD_64             ,GPIO_PORTE_BASE,GPIO_PIN_3}                  // 4-20mA IN 0       IC1
,{SYSCTL_PERIPH_GPIOK, ADC_CTL_CH18|ADC_CTL_SHOLD_64             ,GPIO_PORTK_BASE,GPIO_PIN_2}                  // 4-20mA IN 1 X42   IC2
,{SYSCTL_PERIPH_GPIOK, ADC_CTL_CH17|ADC_CTL_SHOLD_64             ,GPIO_PORTK_BASE,GPIO_PIN_1}                  // 4-20mA IN 2       IC3
,{SYSCTL_PERIPH_GPIOK, ADC_CTL_CH19|ADC_CTL_SHOLD_64             ,GPIO_PORTK_BASE,GPIO_PIN_3}                  // +V_Mon
,{SYSCTL_PERIPH_GPIOD, ADC_CTL_CH15|ADC_CTL_SHOLD_64             ,GPIO_PORTD_BASE,GPIO_PIN_0}                  // I_FAN1 11A
,{SYSCTL_PERIPH_GPIOD, ADC_CTL_CH5 |ADC_CTL_SHOLD_64| ADC_CTL_END,GPIO_PORTD_BASE,GPIO_PIN_6}                  // I_FAN2 11A
,{SYSCTL_PERIPH_GPIOD, ADC_CTL_CH4 |ADC_CTL_SHOLD_64| ADC_CTL_END,GPIO_PORTD_BASE,GPIO_PIN_7}                  // I_FAN3  5A
};

void driverTM4C129X_ADCs_init()
{
    //Adc = new tAdcStructTab[FIELDCOUNT(AdcConfigTab)];
    SysCtlPeripheralReset(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_ADC1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_EIGHTH, 30);
    ADCClockConfigSet(ADC1_BASE, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_EIGHTH, 30);
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);                            // Choose Sequencer 0 and set it at the highest Priority.
    ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);                            // Choose Sequencer 0 and set it at the highest Priority.
    for(uint16_t i=0;i<NUMBER_OF_ADC;i++)
    {
        const tAdcConfigTab* pT=&AdcConfigTab[i];
        uint32_t ADCx_BASE=(i&1)?ADC1_BASE:ADC0_BASE;
        uint32_t Step=i/2;
        SysCtlPeripheralEnable(pT->PeripheralEnablePort);
        GPIOPinTypeADC(pT->ui32PortBase,pT->Pins);
        ADCSequenceStepConfigure(ADCx_BASE,0,Step,pT->ADC_CTL_CHx);
    }
    ADCSequenceEnable(ADC0_BASE, 0);
    ADCSequenceEnable(ADC1_BASE, 0);
}


void HwADCTicker(void *pvParameters)
{
    for( ; ;)
    {
        switch(TickerPhase)
        {
            case 0: // faccio partire a/d
                MacADCProcessorTrigger(ADC0_BASE,0);
                MacADCProcessorTrigger(ADC1_BASE,0);
                AdcIndex = 0;
                TickerPhase++;
                break;
            case 1: // aspetto 1ms
                TickerPhase++;
                break;
            case 2: // leggo i valori, uno ogni 1ms
            default:

                tAdcStructTab *Adc_temp = &Adc[AdcIndex];

                 CommulateValue(Adc_temp, MacADCSequenceDataGet((AdcIndex&1)?ADC1_BASE:ADC0_BASE,0));
                 AdcIndex++;
                 if (AdcIndex >=(int32_t)FIELDCOUNT(AdcConfigTab))
                 {
                     TickerPhase = 0;
                     AdcIndex = 0;
                 }
                 break;
        }
        vTaskDelay(1);
    }
}

int32_t GetADCValue(uint32_t Channel)
{
    tAdcStructTab *Adc_temp = Adc + Channel;

    if(Channel >= FIELDCOUNT(AdcConfigTab))
        return (0x80000000);
    return Adc_temp->value;
}

