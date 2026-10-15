/*
 * cWebBusC2020_ZeroCrossPhaseCut.cpp
 *
 *  Created on: 07.03.2016
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
#include "FreeRTOS.h"
#include "task.h"

#include "driver_ZeroCrossPhaseCut.h"
#include "utility_TM4C129X_.h"


#include "main.h"

#define PWM_PERIOD_5ms 5*(CLOCK/PWM_SYSCLK_DIV/1000)
static int CounterOver = 0;
static int CountInt = 0;
static uint32_t CaptureGood = 0;
static uint32_t ContRec;
static char wait4;
static uint32_t ContatoreMs;
static uint32_t SaltaContinui;

tZeroCrossPhaseCutStructPad ZeroCrossPhaseCut[NUMER_OF_ZERO_CROSS_PHASE_CUT] = {0};


// Timer Capture PB0 95 | PB0-T4CCP0 //+ 42 | PF0-M0PWM0  PWM Out
static const tCaptureConfigTab CaptureConfigTab[]=
        //port activating    ,timer activating    ,pin configure  ,pin of port, port base    ,timer base ,timer  ,base freq
        {{{SYSCTL_PERIPH_GPIOB,SYSCTL_PERIPH_TIMER4,GPIO_PB0_T4CCP0,GPIO_PIN_0,GPIO_PORTB_BASE,TIMER4_BASE,TIMER_A}
         ,{SYSCTL_PERIPH_GPIOF,GPIO_PORTF_BASE,GPIO_PIN_0,SYSCTL_PERIPH_PWM0,GPIO_PF0_M0PWM0,PWM0_BASE,PWM_GEN_0,PWM_OUT_0}}
        };
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// driver for zero cross phase cut via TIMER / PWM
//
// TIMER:  http://datasheet.digitronic.com/ARM/STELLARIS-TEXAS-INSTRUMENTS/TM4/TM4C1294_M4_1024Flash_256SRAM_ETH_MAC_PHY.pdf#page=955
// PWM:  http://datasheet.digitronic.com/ARM/STELLARIS-TEXAS-INSTRUMENTS/TM4/TM4C1294_M4_1024Flash_256SRAM_ETH_MAC_PHY.pdf#page=1678
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setDriverZeroCrossPhaseCutInvert(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp, uint32_t invert)
{
    if(!invert)
        HWREG(ZeroCrossPhaseCutTemp->PwmOInvBase) |= ZeroCrossPhaseCutTemp->PwmPins;
    else
        HWREG(ZeroCrossPhaseCutTemp->PwmOInvBase) &= ~(ZeroCrossPhaseCutTemp->PwmPins);
}

uint32_t getDriverZeroCrossPhaseCutInvert(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp)
{
    return !(HWREG(ZeroCrossPhaseCutTemp->PwmOInvBase) & ZeroCrossPhaseCutTemp->PwmPins);
}

int32_t getDriverZeroCrossPhaseCutValue(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp)
{
    if(ZeroCrossPhaseCutTemp->dClock)
        return ZeroCrossPhaseCutTemp->Offset/(CLOCK/1000000);                                  // return Offset in µs
    return PINERR_NOTREADY;
}

void setDriverZeroCrossPhaseCutValue(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp, int32_t value)
{
    static int32_t OldValueModPwm = -1;
    static int32_t OldValueModTriac = -1;
    int32_t s32Aux;

    if (!ZeroCrossPhaseCutTemp->PulseWidth)
    {
        OldValueModTriac = -1;
        if(ZeroCrossPhaseCutTemp->PwmEnableBase)
        {

            if (value != OldValueModPwm)
            {
                OldValueModPwm = value;
                value*= 1000;

                int32_t n = (muldiv(HAL_HIGH-value,PWM_PERIOD_5ms,HAL_HIGH/2)+1)/2;               // attention value must be inverted, on reason hardware fault

                if( (n>=PWM_PERIOD_5ms) || (n<0) )
                {
                    HWREG(ZeroCrossPhaseCutTemp->PwmxCmpBase)=0;
                    if(HWREG(ZeroCrossPhaseCutTemp->PwmEnableBase) & ZeroCrossPhaseCutTemp->PwmEnableBit)
                    {
                        disable();
                        HWREG(ZeroCrossPhaseCutTemp->PwmEnableBase) &= ~ZeroCrossPhaseCutTemp->PwmEnableBit;
                        enable();
                    }
                }
                else
                {
                    // if(n<0) n=0;
                    HWREG(ZeroCrossPhaseCutTemp->PwmxCmpBase)=n;
                    if(!(HWREG(ZeroCrossPhaseCutTemp->PwmEnableBase) & ZeroCrossPhaseCutTemp->PwmEnableBit))
                    {
                        disable();
                        HWREG(ZeroCrossPhaseCutTemp->PwmEnableBase) |= ZeroCrossPhaseCutTemp->PwmEnableBit;
                        enable();
                    }
                }
            }
        }


    }
    else
    {
        OldValueModPwm = -1;
        if (value != OldValueModTriac)
        {
            OldValueModTriac = value;
            if(value>25000 || value<-25000 || (value==0) ) // MNS aggiunto valore 0
            {
                ZeroCrossPhaseCutTemp->Enable=0;
            }
            else
            {
                // { ADD MNS STUACW04.01
                if (ZeroCrossPhaseCutTemp->PulseWidth==1000)
                {
                    if ((value)>9500)
                    {
                        value = 9500;
                    }
                    else if ((value) < 1500)
                    {
                        value = 1500;
                    }
                }
                else
                {
                    if ((value)>9500)
                    {
                        value = 9500;
                    }
                    else if ((value) < 2500)
                    {
                        value = 2500;
                    }

                }
                // }

                s32Aux = (value - ZeroCrossPhaseCutTemp->PulseWidth)*(CLOCK/1000000);            // MNS forse andrebbe differenziato tra 50Hz e 60Hz // tolto offset di 1000 perche' c'era 1 ms di differenza con la sb7

                disable();
                ZeroCrossPhaseCutTemp->Offset = s32Aux;                                   // phase offset in µs  // MOD MNS
                ZeroCrossPhaseCutTemp->Enable=1;
                enable();
            }
        }
    }
}

void setDriverZeroCrossPhaseCutPulseWidth(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp, int32_t value)
{
    const tCaptureConfigTab* pT=&CaptureConfigTab[0];


        if (value==(int32_t)ZeroCrossPhaseCutTemp->PulseWidth) // valore già impostato
        {

        }
        else if (value==0)  // si utilizza PWM con periodo 5 ms, non si usa zero cross
        {
            ZeroCrossPhaseCutTemp->PulseWidth = 0;
            driverZeroCrossPWM_init(ZeroCrossPhaseCutTemp, pT);   // inizializzo pwm
            ZeroCrossPhaseCutTemp->Enable = 0;
        }
        else // si imposta lunghezza impulso da zero crossing
        {                                                     // PWM gen enable

            if((value >= 50) && (value <= 3000))
            {
                if (ZeroCrossPhaseCutTemp->PwmxCmpBase)
                {
                    // Se veniamo dal pwm ritorniamo alla modalita' invertita

                    if (ZeroCrossPhaseCutTemp->PulseWidth == 0)
                    {
                        setDriverZeroCrossPhaseCutInvert(ZeroCrossPhaseCutTemp, 0);
                    }

                    disable();
                    HWREG(ZeroCrossPhaseCutTemp->PwmxCmpBase) = 0;
                    enable();
                }
                ZeroCrossPhaseCutTemp->PulseWidth = value;
            }
            else // prima volta
            {
                ZeroCrossPhaseCutTemp->PulseWidth = PHASECUT_PULSE_WIDTH_DEFAULT;
            }
        }
}

int32_t getDriverZeroCrossPhaseCutHz1000(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp)
{
    // { MOD MNS STUPCW09.00 ECR 6330 gestione ACFAN come ingresso digitale
    uint32_t h = ZeroCrossPhaseCutTemp->dClock;

    if(!h)
        return PINERR_OPEN;
    return CLOCK*2/256*1000/(h/256);
    // }
}

void HwDriverZeroCrossPhaseCutTicker(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp)
{
#define Clock47p5hHz    CLOCK*4/98        // Clocks for 4 half cycles at 50Hz -2,5Hz  // MOD MNS 2%
#define Clock52p5hHz    CLOCK*4/102       // Clocks for 4 half cycles at 50Hz +2,5Hz  // MOD MNS 2%
#define Clock57p5hHz    CLOCK*4/112       // Clocks for 4 half cycles at 60Hz -2,5Hz  // MOD MNS 2%
#define Clock62p5hHz    CLOCK*4/122       // Clocks for 4 half cycles at 60Hz +2,5Hz  // MOD MNS 2%
#define Clock50Hz       CLOCK*4/100


    uint32_t CaptureOld;
    CaptureOld = ZeroCrossPhaseCutTemp->CaptureNow;
    ZeroCrossPhaseCutTemp->CaptureNow = HWREG(ZeroCrossPhaseCutTemp->TimerCaptureBase);            // get last captures value


    if (ZeroCrossPhaseCutTemp->CaptureNow != CaptureOld)
    {

        ContatoreMs = 0;

    }

    ContatoreMs++;


    uint16_t Inx = ZeroCrossPhaseCutTemp->CaptureLastInInx;            // get inx for last stored capture
    // if(CaptureNow!=Captures[Inx])          // has captures value changed?


    // CaptureOld = Captures[Inx];
    CountInt++;


    if ( (ZeroCrossPhaseCutTemp->CaptureNow != CaptureOld) && (CountInt > 7) )
    {

        CountInt = 0;

        Inx = (Inx+1)%FIELDCOUNT(ZeroCrossPhaseCutTemp->Captures);                               // find next index for storing
        ZeroCrossPhaseCutTemp->CaptureLastInInx = Inx;
        uint32_t CaptureBefore= ZeroCrossPhaseCutTemp->Captures[Inx];                                // get captured value 4 times before
        ZeroCrossPhaseCutTemp->Captures[Inx] = ZeroCrossPhaseCutTemp->CaptureNow;
        uint32_t dClockTmp=(ZeroCrossPhaseCutTemp->CaptureNow - CaptureBefore)&0xFFFFFF;              // calculate clock difference

        if((dClockTmp < Clock47p5hHz && dClockTmp > Clock52p5hHz) && (!CfPowFreq) )
        { // 50Hz
            CounterOver = 0;
            CaptureGood = ZeroCrossPhaseCutTemp->CaptureNow;
            ZeroCrossPhaseCutTemp->dClock = dClockTmp;
            ZeroCrossPhaseCutTemp->WasGood = 1;
            ZeroCrossPhaseCutTemp->ms_behind_zerocross=0;
            //C2020Data.Web_Debug[13] = CLOCK*2/256*1000/(dClock/256);;

        }

        else if ((dClockTmp < Clock57p5hHz && dClockTmp > Clock62p5hHz)  && (CfPowFreq) )
        { // 60Hz

            CounterOver = 0;
            CaptureGood = ZeroCrossPhaseCutTemp->CaptureNow;
            ZeroCrossPhaseCutTemp->dClock = dClockTmp;
            ZeroCrossPhaseCutTemp->WasGood = 1;
            ZeroCrossPhaseCutTemp->ms_behind_zerocross=0;
            // WasGood = 1;
        }

        else
        { // async!

            ZeroCrossPhaseCutTemp->WasGood = 0;

            // dClock=0;    // COMM MNS


        }

    }
    else if (ZeroCrossPhaseCutTemp->CaptureNow != CaptureOld)
    {
        // C2020Data.ZeroCrossingError++;

    }
    if((++ZeroCrossPhaseCutTemp->ms_behind_zerocross >= ZEROCROSS_TIMEOUT) || (ContatoreMs >= 1000) )                                // rise up zero cross time
    {
        // C2020Data.ZeroCrossingFatalError++;
        ZeroCrossPhaseCutTemp->dClock = 0;                 // ADD MNS set here instead of async case
        ZeroCrossPhaseCutTemp->ms_behind_zerocross = ZEROCROSS_TIMEOUT;                                  // max to ZEROCROSS_TIMEOUT
        ContatoreMs = 1000;
        //C2020Data.Web_Debug[12]++;
    }
    else
    {

    }


    if (!ZeroCrossPhaseCutTemp->PulseWidth)
    {
        // si usa un pwm con periodo 5 ms, gestito sotto set_value

    }
    else if(ZeroCrossPhaseCutTemp->dClock && ZeroCrossPhaseCutTemp->Enable && ZeroCrossPhaseCutTemp->ms_behind_zerocross < ZEROCROSS_TIMEOUT)               // is phase cut enable?
    {
        // if ( (ms_behind_zerocross==4) && WasGood )                                              // yes -> is ~4ms behind zerocross?
         if (( (ZeroCrossPhaseCutTemp->ms_behind_zerocross==4) && ZeroCrossPhaseCutTemp->WasGood ) ||  ((ZeroCrossPhaseCutTemp->ms_behind_zerocross>40) &&  ContatoreMs == 4) )      // MOD MNS STUACW04.01 prendo ugualmente lo zero crossing anche se non ne ho trovati 4 consecutivi buoni
        {
            int32_t PwmVal;                                                         // yes -> calc new phase cut sync
            int32_t TimValue;
            int32_t TimCapt;
            disable();
            PwmVal = HWREG(ZeroCrossPhaseCutTemp->PwmxCountBase);
            // TimCapt=HWREG(TimerCaptureBase);
            TimValue = HWREG(ZeroCrossPhaseCutTemp->TimerValueBase);
            enable();
            //TimCapt = CaptureNow; // MNS uso l'ultimo valore buono inutile ricalcolarlo
            if (ZeroCrossPhaseCutTemp->ms_behind_zerocross != 4)
            {
                TimCapt = ZeroCrossPhaseCutTemp->CaptureNow;   // MNS uso l'ultimo valore anche se non sono sicuro sia proprio buono

            }
            else
                TimCapt = CaptureGood;  // MNS uso l'ultimo valore buono

            TimValue = TimValue-TimCapt;                                          // get measuered time in timer phase now in CPU clocks

            if(TimValue<0) TimValue+=0x1000000;                                 // TimValue thinks in 24 bit


            TimValue += ZeroCrossPhaseCutTemp->Offset;                                                   // add user offset


            TimValue += PwmVal*32;                                                // get difference between Timer Phase and Pwm Phase
            TimValue *= -8;                                                       // preparing for compare


            while(TimValue<-(int32_t)(ZeroCrossPhaseCutTemp->dClock))    TimValue += ZeroCrossPhaseCutTemp->dClock*2;               // fixing position of pulse in cpuclocks from zerocross
            while(TimValue>(int32_t)(ZeroCrossPhaseCutTemp->dClock))     TimValue -= ZeroCrossPhaseCutTemp->dClock*2;               // fixing position of pulse in cpuclocks from zerocross




  /*
            while(TimValue<-(s32)(Clock50Hz))    TimValue+=Clock50Hz*2;               // fixing position of pulse in cpuclocks from zerocross
            while(TimValue>(s32)(Clock50Hz))     TimValue-=Clock50Hz*2;               // fixing position of pulse in cpuclocks from zerocross
 */
            HWREG(ZeroCrossPhaseCutTemp->PwmxLoadBase) = (uint32_t)(TimValue + ZeroCrossPhaseCutTemp->dClock*2)/256ul;                 // Sync dClock
            if(ZeroCrossPhaseCutTemp->Enable && !HWREG(ZeroCrossPhaseCutTemp->PwmxCmpBase))
            {
                HWREG(ZeroCrossPhaseCutTemp->PwmxCmpBase)=((CLOCK/1000000)*ZeroCrossPhaseCutTemp->PulseWidth)/PWM_SYSCLK_DIV;     // set Pulsewith in µs

            }



        }
    }
    else if(HWREG(ZeroCrossPhaseCutTemp->PwmxCmpBase))                                                 // no -> phasecut disable
        HWREG(ZeroCrossPhaseCutTemp->PwmxCmpBase)=0;                                                   // no Pulsewith if disable



    //C2020Data.Web_Debug[16] = dClock+1;
}

void HwDriverZCPCTicker(void *pvParameters)
{
    for( ; ; )
    {
        for(uint32_t i=0; i < NUMER_OF_ZERO_CROSS_PHASE_CUT; i++)
        {
            HwDriverZeroCrossPhaseCutTicker(&ZeroCrossPhaseCut[i]);
        }
        vTaskDelay(1);
    }
}

void driverZeroCrossPhaseCut(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp)
{
#define Channel 0
    setDriverZeroCrossPhaseCutPulseWidth(ZeroCrossPhaseCutTemp, PHASECUT_PULSE_WIDTH_DEFAULT);

    const tCaptureConfigTab* pT=&CaptureConfigTab[Channel];
    ZeroCrossPhaseCutTemp->TimerCaptureBase = pT->Capture.ui32TimerBase+(pT->Capture.ui32Timer==TIMER_A?TIMER_O_TAR:TIMER_O_TBR);
    ZeroCrossPhaseCutTemp->TimerValueBase = pT->Capture.ui32TimerBase+(pT->Capture.ui32Timer==TIMER_A?TIMER_O_TAV:TIMER_O_TBV);
    SysCtlPeripheralEnable(pT->Capture.PeripheralEnablePort);                                               // activate port
    SysCtlPeripheralEnable(pT->Capture.PeripheralEnableTimer);                                              // activate Timer
    GPIOPinConfigure(pT->Capture.PinConfig);
    GPIOPinTypeTimer(pT->Capture.ui32PortBase,pT->Capture.Pins);                                                    // PWM1,PWM2,PWM3 port D3,D1,D2

    if(!(HWREG(pT->Capture.ui32TimerBase+TIMER_O_CTL) & (TIMER_CTL_TAEN | TIMER_CTL_TBEN)))                     // is one of A or B already configured
    {
        TimerConfigure   (pT->Capture.ui32TimerBase,TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_CAP_TIME_UP|TIMER_CFG_B_CAP_TIME_UP);     // no -> PWM1: D2 T1CCP0 (Pin3);   PWM3: D3 T1CCP1 (PIN4)
        TimerControlEvent(pT->Capture.ui32TimerBase,TIMER_BOTH,TIMER_EVENT_NEG_EDGE);
        HWREG(pT->Capture.ui32TimerBase + TIMER_O_CC)=TIMER_CLOCK_SYSTEM;
    }
    TimerDisable        (pT->Capture.ui32TimerBase,pT->Capture.ui32Timer);
    TimerPrescaleSet    (pT->Capture.ui32TimerBase,pT->Capture.ui32Timer,0xFF);
    TimerEnable         (pT->Capture.ui32TimerBase,pT->Capture.ui32Timer);

    ZeroCrossPhaseCutTemp->PwmOInvBase = pT->Pwm.ui32PwmBase+PWM_O_INVERT;
    ZeroCrossPhaseCutTemp->PwmEnableBase = pT->Pwm.ui32PwmBase+PWM_O_ENABLE;
    ZeroCrossPhaseCutTemp->PwmEnableBit  = 1<<(pT->Pwm.PwmOut&7);
    ZeroCrossPhaseCutTemp->PwmxCmpBase = pT->Pwm.ui32PwmBase+(pT->Pwm.PwmOut&0xFFFFFFC0)+((pT->Pwm.PwmOut&1)?PWM_O_X_CMPB:PWM_O_X_CMPA);
    ZeroCrossPhaseCutTemp->PwmxLoadBase = pT->Pwm.ui32PwmBase+(pT->Pwm.PwmOut&0xFFFFFFC0)+PWM_O_X_LOAD;
    ZeroCrossPhaseCutTemp->PwmxCountBase = pT->Pwm.ui32PwmBase+(pT->Pwm.PwmOut&0xFFFFFFC0)+PWM_O_X_COUNT;

    ZeroCrossPhaseCutTemp->PwmPins = pT->Pwm.Pins;
    SysCtlPeripheralEnable(pT->Pwm.PeripheralEnablePort);                                                   // activate port
    SysCtlPeripheralEnable(pT->Pwm.PeripheralEnablePwm);                                                    // activate PWM
    GPIOPinConfigure(pT->Pwm.PinConfig);
    GPIOPinTypePWM(pT->Pwm.ui32PortBase,pT->Pwm.Pins);                                                      // set pin for out PWM
    PWMClockSet(pT->Pwm.ui32PwmBase,PWM_SYSCLK_DIV_x);                                                      // set clock to 120/32 MHz for ca. 10ms basefreq
    PWMGenConfigure(pT->Pwm.ui32PwmBase,pT->Pwm.PwmGen,PWM_GEN_MODE_DBG_RUN);                               // PWM config
    PWMGenPeriodSet(pT->Pwm.ui32PwmBase,pT->Pwm.PwmGen,(CLOCK/PWM_SYSCLK_DIV/100));                         // PWM set default period to 10ms
    PWMPulseWidthSet(pT->Pwm.ui32PwmBase,pT->Pwm.PwmOut,0);                                                 // PWM set pulsewith to 0%                                                               // PWM non inverting
    setDriverZeroCrossPhaseCutInvert(ZeroCrossPhaseCutTemp, 0);
    PWMGenEnable(pT->Pwm.ui32PwmBase,pT->Pwm.PwmGen);                                                       // PWM gen enable
    HWREG(ZeroCrossPhaseCutTemp->PwmxCmpBase) = 0;                                                                                       // No pulsewith for start
}

// { ADD MNS

void driverZeroCrossPWM_init(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp, const tCaptureConfigTab* pT)
{
    SysCtlPeripheralEnable(pT->Pwm.PeripheralEnablePort);                           // activate port
    SysCtlPeripheralEnable(pT->Pwm.PeripheralEnablePwm);                            // activate PWM
    GPIOPinConfigure(pT->Pwm.PinConfig);
    GPIOPinTypePWM(pT->Pwm.ui32PortBase,pT->Pwm.Pins);                                      // set pin for out PWM
    PWMClockSet(pT->Pwm.ui32PwmBase,PWM_SYSCLK_DIV_x);                                  // set clock to 60 MHz for 1ms basefreq
    PWMGenConfigure(pT->Pwm.ui32PwmBase,pT->Pwm.PwmGen,PWM_GEN_MODE_DBG_RUN);               // PWM config
    PWMGenPeriodSet(pT->Pwm.ui32PwmBase,pT->Pwm.PwmGen,PWM_PERIOD_5ms);                     // PWM set period to 1ms
    PWMPulseWidthSet(pT->Pwm.ui32PwmBase,pT->Pwm.PwmOut,0);                                 // PWM set pulsewith to 0%
    PWMGenEnable(pT->Pwm.ui32PwmBase,pT->Pwm.PwmGen);                                       // PWM gen enable
    PWMOutputState(pT->Pwm.ui32PwmBase,pT->Pwm.PwmOut,1);
    setDriverZeroCrossPhaseCutInvert(ZeroCrossPhaseCutTemp, 1);                             // out enable
    uint32_t PwmEnableBase =pT->Pwm.ui32PwmBase+PWM_O_ENABLE;
    uint32_t PwmEnableBit  =1<<(pT->Pwm.PwmOut&7);
    HWREG(PwmEnableBase) &= ~PwmEnableBit;
}

void driver_zcpc()
{
    for(uint16_t i = 0; i < NUMER_OF_ZERO_CROSS_PHASE_CUT; i++)
    {
        driverZeroCrossPhaseCut(&ZeroCrossPhaseCut[i]);
    }
}
//}
