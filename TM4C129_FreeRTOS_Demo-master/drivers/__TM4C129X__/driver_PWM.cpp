/*
 * cWebBusC2020_PWM.cpp
 *
 *  Created on: 01.12.2015
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

#include "driver_PWM.h"
#include "utility_TM4C129X_.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// driver for PWM via PWM
//
// PWM:  http://datasheet.digitronic.com/ARM/STELLARIS-TEXAS-INSTRUMENTS/TM4/TM4C1294_M4_1024Flash_256SRAM_ETH_MAC_PHY.pdf#page=1678
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tPwmStructTab Pwm[NUMER_OF_PWM_OUTPUT] = {0};


#define PWM_PERIOD_1ms (CLOCK/PWM_SYSCLK_DIV/1000)

uint32_t getPWMValue(tPwmStructTab * Pwm)
{
    if(Pwm->PwmEnableBase)
    {
        if(HWREG(Pwm->PwmEnableBase) & Pwm->PwmEnableBit)
            return HAL_HIGH-muldiv(HWREG(Pwm->PwmxCmpBase),HAL_HIGH,PWM_PERIOD_1ms);     // attention value must be inverted, on reason hardware fault
        return HAL_HIGH-HAL_HIGH;                                                   // attention value must be inverted, on reason hardware fault
    }
    return PINERR_NOTEXISTS;
}

void setPwmValue(tPwmStructTab * Pwm, int32_t value)
{
    if(Pwm->PwmEnableBase)
    {
        int32_t n=(muldiv(HAL_HIGH-value,PWM_PERIOD_1ms,HAL_HIGH/2)+1)/2;               // attention value must be inverted, on reason hardware fault
        if(n>=PWM_PERIOD_1ms)
        {
            if(HWREG(Pwm->PwmEnableBase) & Pwm->PwmEnableBit)
            {
                disable();
                HWREG(Pwm->PwmEnableBase) &= ~Pwm->PwmEnableBit;
                enable();
            }
        }
        else
        {
            if(n<0) n=0;
            HWREG(Pwm->PwmxCmpBase)=n;
            if(!(HWREG(Pwm->PwmEnableBase) & Pwm->PwmEnableBit))
            {
                disable();
                HWREG(Pwm->PwmEnableBase) |= Pwm->PwmEnableBit;
                enable();
            }
        }
    }
}

typedef struct
{
    uint32_t PeripheralEnablePort;
    uint32_t ui32PortBase;
    uint32_t Pins;
    uint32_t PeripheralEnablePwm;
    uint32_t PinConfig;
    uint32_t ui32PwmBase;
    uint32_t PwmGen;
    uint32_t PwmOut;
}tPwmConfigTab;

static const tPwmConfigTab PwmConfigTab[]=
        //port activating    ,timer activating    ,pin configure  ,pin of port, port base    ,timer base ,timer  ,base freq
        {{SYSCTL_PERIPH_GPIOG,GPIO_PORTG_BASE,GPIO_PIN_0,SYSCTL_PERIPH_PWM0,GPIO_PG0_M0PWM4,PWM0_BASE,PWM_GEN_2,PWM_OUT_4}
        ,{SYSCTL_PERIPH_GPIOG,GPIO_PORTG_BASE,GPIO_PIN_1,SYSCTL_PERIPH_PWM0,GPIO_PG1_M0PWM5,PWM0_BASE,PWM_GEN_2,PWM_OUT_5}
        ,{SYSCTL_PERIPH_GPIOK,GPIO_PORTK_BASE,GPIO_PIN_5,SYSCTL_PERIPH_PWM0,GPIO_PK5_M0PWM7,PWM0_BASE,PWM_GEN_3,PWM_OUT_7}
        };

void driverPwm_init(const tPwmConfigTab* pT)
{
    SysCtlPeripheralEnable(pT->PeripheralEnablePort);                           // activate port
    SysCtlPeripheralEnable(pT->PeripheralEnablePwm);                            // activate PWM
    GPIOPinConfigure(pT->PinConfig);
    GPIOPinTypePWM(pT->ui32PortBase,pT->Pins);                                      // set pin for out PWM
    PWMClockSet(pT->ui32PwmBase,PWM_SYSCLK_DIV_x);                                  // set clock to 60 MHz for 1ms basefreq
    PWMGenConfigure(pT->ui32PwmBase,pT->PwmGen,PWM_GEN_MODE_DBG_RUN);               // PWM config
    PWMGenPeriodSet(pT->ui32PwmBase,pT->PwmGen,PWM_PERIOD_1ms);                     // PWM set period to 1ms
    PWMPulseWidthSet(pT->ui32PwmBase,pT->PwmOut,0);                                 // PWM set pulsewith to 0%
    PWMGenEnable(pT->ui32PwmBase,pT->PwmGen);                                       // PWM gen enable
    PWMOutputState(pT->ui32PwmBase,pT->PwmOut,1);                                   // out enable

    uint32_t PwmEnableBase =pT->ui32PwmBase + PWM_O_ENABLE;
    uint32_t PwmEnableBit  =1<<(pT->PwmOut&7);
    HWREG(PwmEnableBase) &= ~PwmEnableBit;
}

void driverPWM(tPwmStructTab * Pwm, uint32_t Channel)
{
    if(Channel>=FIELDCOUNT(PwmConfigTab))
        Pwm->PwmEnableBase =0;
    else
    {
        const tPwmConfigTab* pT=&PwmConfigTab[Channel];
        Pwm->PwmEnableBase =pT->ui32PwmBase+PWM_O_ENABLE;
        Pwm->PwmEnableBit  =1<<(pT->PwmOut&7);
        Pwm->PwmxCmpBase=pT->ui32PwmBase+(pT->PwmOut&0xFFFFFFC0)+((pT->PwmOut&1)?PWM_O_X_CMPB:PWM_O_X_CMPA);
        driverPwm_init(pT);
    }
}

void init_driver_pwm()
{
    for(uint16_t i = 0; i < NUMER_OF_PWM_OUTPUT; i++)
    {
        driverPWM(&Pwm[i], i);
    }
}

/*
void FastHardwareInit(void)                                              // patch for inverted physical hardware for faster initialisation
{
    const tPwmConfigTab* pT;
    STRUC_FOR(pT,PwmConfigTab)
    driverPwm_init(pT);                                                   // be careful, cWebBusC2020_PWM is not initilazied!
}
*/
