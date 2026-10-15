/*
 * cWebBusC2020TimerPWM.cpp
 *
 *  Created on: 03.03.2016
 *      Author: us
 */

#include "stdint.h"
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
#include "timer.h"

#include "driver_TimerPWM.h"
#include "utility_TM4C129X_.h"


#define TIMERPWMCLOCK                                   16000000

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// driver for PWM via TIMER
//
// TIMER:  http://datasheet.digitronic.com/ARM/STELLARIS-TEXAS-INSTRUMENTS/TM4/TM4C1294_M4_1024Flash_256SRAM_ETH_MAC_PHY.pdf#page=955
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tTimerPwmStructTab TimerPwm[NUM_TIMER_PWM] = {0};

int32_t getTimerPwmGetValue(tTimerPwmStructTab * TimerPwmTemp)
{
    int32_t Load = TimerLoadGet(TimerPwmTemp->ui32Base, TimerPwmTemp->ui32Timer);
    int32_t Match = TimerMatchGet(TimerPwmTemp->ui32Base, TimerPwmTemp->ui32Timer);
    return muldiv(Match,HAL_HIGH,Load);
}

void setTimerPwmGetValue(tTimerPwmStructTab * TimerPwmTemp, int32_t value)
{
    long Load = TimerLoadGet(TimerPwmTemp->ui32Base, TimerPwmTemp->ui32Timer);
    long Match = muldiv(value,Load,HAL_HIGH);
    if(Match>Load)      Match=Load;
    if(Match<0)         Match=0;
    if(!Match&&value>0) Match=1;
    TimerMatchSet(TimerPwmTemp->ui32Base, TimerPwmTemp->ui32Timer, Match);
}

typedef struct
{
    uint32_t PeripheralEnablePort;
    uint32_t PeripheralEnableTimer;
    uint32_t PinConfig;
    uint32_t Pins;
    uint32_t ui32PortBase;
    uint32_t ui32TimerBase;
    uint32_t ui32Timer;
    uint32_t BaseFreq;
}tPwmConfigTab;

static const tPwmConfigTab PwmConfigTab[]=
        //port activating    ,timer activating    ,pin configure  ,pin of port, port base    ,timer base ,timer  ,base freq
        {{SYSCTL_PERIPH_GPIOM,SYSCTL_PERIPH_TIMER2,GPIO_PM1_T2CCP1,GPIO_PIN_1,GPIO_PORTM_BASE,TIMER2_BASE,TIMER_B,10000}
        ,{SYSCTL_PERIPH_GPIOM,SYSCTL_PERIPH_TIMER2,GPIO_PM0_T2CCP0,GPIO_PIN_0,GPIO_PORTM_BASE,TIMER2_BASE,TIMER_A,10000}
        };

void TimerPwmInit(tTimerPwmStructTab *TimerPwmtemp, uint32_t Channel)
{
    if(Channel>=FIELDCOUNT(PwmConfigTab))
        TimerPwmtemp->ui32Base =0;
    else
    {
        const tPwmConfigTab* pT=&PwmConfigTab[Channel];
        TimerPwmtemp->ui32Base = pT->ui32TimerBase;
        TimerPwmtemp->ui32Timer = pT->ui32Timer;
        SysCtlPeripheralEnable(pT->PeripheralEnablePort);                                               // activate port
        SysCtlPeripheralEnable(pT->PeripheralEnableTimer);                                              // activate Timer
        GPIOPinConfigure(pT->PinConfig);
        GPIOPinTypeTimer(pT->ui32PortBase,pT->Pins);                                                    // PWM1,PWM2,PWM3 port D3,D1,D2

        if(!(HWREG(pT->ui32TimerBase+TIMER_O_CTL) & (TIMER_CTL_TAEN | TIMER_CTL_TBEN)))                     // is one of A or B already configured
        {
            TimerConfigure(pT->ui32TimerBase,TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM|TIMER_CFG_B_PWM);     // no -> PWM1: D2 T1CCP0 (Pin3);   PWM3: D3 T1CCP1 (PIN4)
            TimerControlLevel(pT->ui32TimerBase, TIMER_BOTH, 1);
            HWREG(pT->ui32TimerBase + TIMER_O_CC)=TIMER_CLOCK_PIOSC;
        }
        uint32_t Prescaler=(TIMERPWMCLOCK/(((pT->BaseFreq)-1)*0x10000));
        uint32_t Load=((TIMERPWMCLOCK/(Prescaler+1))/(pT->BaseFreq));
        TimerDisable       (TimerPwmtemp->ui32Base, TimerPwmtemp->ui32Timer);
        TimerPrescaleSet   (TimerPwmtemp->ui32Base, TimerPwmtemp->ui32Timer, Prescaler);
        TimerMatchSet      (TimerPwmtemp->ui32Base, TimerPwmtemp->ui32Timer, 0);
        TimerLoadSet       (TimerPwmtemp->ui32Base, TimerPwmtemp->ui32Timer, Load);
        HWREG(TimerPwmtemp->ui32Base + (((TimerPwmtemp->ui32Timer) == TIMER_A)?TIMER_O_TAMR:TIMER_O_TBMR))|=(((TimerPwmtemp->ui32Timer) == TIMER_A)?(TIMER_TAMR_TAPLO|TIMER_TAMR_TAMRSU):(TIMER_TBMR_TBPLO|TIMER_TBMR_TBMRSU));
        TimerEnable(TimerPwmtemp->ui32Base, TimerPwmtemp->ui32Timer);
    }
}

void driverTimerPwm()
{
    for(uint32_t i = 0; i<NUM_TIMER_PWM; i++)
    {
        TimerPwmInit(&TimerPwm[i], i);
    }
}





