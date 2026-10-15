/*
 * cWebBusC2020TimerCounter.cpp
 *
 *  Created on: 04.03.2016
 *      Author: us
 */

#include "stdint.h"
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
#include "FreeRTOS.h"
#include "task.h"

#include "driver_TimerCounter.h"
#include "utility_TM4C129X_.h"


#define TIMERPWMCLOCK                                   16000000

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// driver for Counter via TIMER
//
// TIMER:  http://datasheet.digitronic.com/ARM/STELLARIS-TEXAS-INSTRUMENTS/TM4/TM4C1294_M4_1024Flash_256SRAM_ETH_MAC_PHY.pdf#page=955
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tTimerStructTab Timer[NUMBER_OF_TIMER] = {0};

void HwTimerTick(void *pvParameters)
{
    for( ; ;)
    {
        if(Timer != 0)
        {
            for(uint16_t i = 0; i<NUMBER_OF_TIMER; i++)
            {
                HwTimerTicker(&Timer[i]);
            }
        }
        vTaskDelay(1);
    }
}


void HwTimerTicker(tTimerStructTab *Timer_temp)
{
    TickType_t CountAct=HWREG(Timer_temp->TimerValueBase);
    if(!Timer_temp->CountInitialized)
    {
        Timer_temp->CountInitialized=true;
        Timer_temp->CountLast=CountAct;
    }
    else if(++Timer_temp->CountTicker>=1000)
    {
        Timer_temp->CountTicker=0;
        Timer_temp->ImpPerSec=((uint16_t)(CountAct - Timer_temp->CountLast))/2;
        Timer_temp->CountLast=CountAct;
    }
}

uint32_t getTimerValue(tTimerStructTab *Timer_temp)
{
    return Timer_temp->ImpPerSec;
}

void setTimerValue(tTimerStructTab *Timer_temp, uint32_t value)
{

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
}tPwmConfigTab;

static const tPwmConfigTab PwmConfigTab[]=
        //port activating    ,timer activating    ,pin configure  ,pin of port, port base    ,timer base ,timer  ,base freq
        {{SYSCTL_PERIPH_GPIOD,SYSCTL_PERIPH_TIMER3,GPIO_PD4_T3CCP0,GPIO_PIN_4,GPIO_PORTD_BASE,TIMER3_BASE,TIMER_A}
        ,{SYSCTL_PERIPH_GPIOD,SYSCTL_PERIPH_TIMER3,GPIO_PD5_T3CCP1,GPIO_PIN_5,GPIO_PORTD_BASE,TIMER3_BASE,TIMER_B}
        ,{SYSCTL_PERIPH_GPIOM,SYSCTL_PERIPH_TIMER5,GPIO_PM7_T5CCP1,GPIO_PIN_7,GPIO_PORTM_BASE,TIMER5_BASE,TIMER_B}
        };

void driverTimerCounter(tTimerStructTab *Timer_temp, uint16_t Channel)
{
    Timer_temp = Timer_temp + Channel;
    if(Channel<FIELDCOUNT(PwmConfigTab))
    {
        const tPwmConfigTab* pT = &PwmConfigTab[Channel];
        Timer_temp->TimerValueBase = pT->ui32TimerBase + (pT->ui32Timer==TIMER_A?TIMER_O_TAR:TIMER_O_TBR);
        SysCtlPeripheralEnable(pT->PeripheralEnablePort);                                               // activate port
        SysCtlPeripheralEnable(pT->PeripheralEnableTimer);                                              // activate Timer
        GPIOPinConfigure(pT->PinConfig);
        GPIOPinTypeTimer(pT->ui32PortBase,pT->Pins);                                                    // PWM1,PWM2,PWM3 port D3,D1,D2

        if(!(HWREG(pT->ui32TimerBase+TIMER_O_CTL) & (TIMER_CTL_TAEN | TIMER_CTL_TBEN)))                     // is one of A or B already configured
        {
            TimerConfigure   (pT->ui32TimerBase,TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_CAP_COUNT_UP|TIMER_CFG_B_CAP_COUNT_UP);     // no -> PWM1: D2 T1CCP0 (Pin3);   PWM3: D3 T1CCP1 (PIN4)
            TimerControlLevel(pT->ui32TimerBase,TIMER_BOTH, 1);
            TimerControlEvent(pT->ui32TimerBase,TIMER_BOTH,TIMER_EVENT_BOTH_EDGES);
            HWREG(pT->ui32TimerBase + TIMER_O_CC) = TIMER_CLOCK_PIOSC;
        }
        TimerDisable        (pT->ui32TimerBase,pT->ui32Timer);
        TimerPrescaleSet    (pT->ui32TimerBase,pT->ui32Timer,0);
        TimerMatchSet       (pT->ui32TimerBase,pT->ui32Timer,0xFFFF);
        TimerLoadSet        (pT->ui32TimerBase,pT->ui32Timer,0xFFFF);
        HWREG(pT->ui32TimerBase + (((pT->ui32Timer) == TIMER_A)?TIMER_O_TAMR:TIMER_O_TBMR))|=(((pT->ui32Timer) == TIMER_A)?(TIMER_TAMR_TAPLO|TIMER_TAMR_TAMRSU):(TIMER_TBMR_TBPLO|TIMER_TBMR_TBMRSU));
        TimerEnable         (pT->ui32TimerBase,pT->ui32Timer);
    }
}

void driverTimerCounterInit()
{
    for(uint16_t i = 0; i<NUMBER_OF_TIMER; i++)
    {
        driverTimerCounter(&Timer[i], i);
    }
}



