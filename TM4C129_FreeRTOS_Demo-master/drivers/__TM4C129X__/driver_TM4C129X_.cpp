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

#define BASE_SLOTS                 3
#define BASE_CHANNELS_PER_SLOT     16

#define MAX_SLOTS                  16       // und einem Slot abgebildet

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class for slot type C2020_BASE0
//
// channel  typ             physical unit           WebBusTyp                Name         CpuPin|Port-Funct(t10-p-b1)
//  0       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R1-X9            2 | PD1
//  1       digital IO      0,00000%|100,00000%     WEBBUS_PIN_OUT24_PNP                    105 | PP4| DBUS --  106 | PP5| DBUS
//  2       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R3-X10          76 | PM2)
//  3       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R4-X10          75 | PM3)
//  4       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R5-X11          74 | PM4)
//  5       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R6-X11          73 | PM5)
//  6       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R7-X11          72 | PM6)
//  7       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R8-X11          81 | PL0
//  8       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R9-X11          82 | PL1
//  9       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R10-X11         83 | PL2
// 10       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R11-X11         84 | PL3
// 11       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R12-X11         94 | PL6
// 12       digital out     0,00000%|100,00000%     WEBBUS_PIN_OUT230        R13-X11         93 | PL7
// 13       analog out      0,00000%-100,00000%     WEBBUS_PIN_AOUT_PWM      PWM1-FAN1-X17   49 | PG0-M0PWM4
// 14       analog out      0,00000%-100,00000%     WEBBUS_PIN_AOUT_PWM      PWM3-FAN2-X18   50 | PG1-M0PWM5
// 15       analog out      0,00000%-100,00000%     WEBBUS_PIN_AOUT_PWM      PWM2-FAN3-X19   62 | PK5-M0PWM7
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class for slot type C2020_BASE1
//
// channel  typ             physical unit           WebBusTyp                Name      CpuPin|Port-Funct(t10-p-B1)
//  0       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN1-X5     3 | PD2
//  1       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN2-X34    4 | PD3
//  2       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN3-X5    25 | PC4
//  3       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN4-X5    24 | PC5
//  4       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN5-X15   23 | PC6
//  5       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN6-X13   22 | PC7
//  6       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN7-X12   44 | PF2
//  7       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN8-X35   45 | PF3
//  8       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN9-X39   30 | PH1
//  9       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN10-X40  31 | PH2
// 10       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN11-X37  32 | PH3
// 11       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN12-X38  61 | PK6
// 12       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      DIGIN13-X36  60 | PK7
// 13       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      TACH-1-X17  125 | PD4-T3CCP0
// 14       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      TACH-2-X18  126 | PD5-T3CCP1
// 15       digital in      0,00000%|100,00000%     WEBBUS_PIN_IN24_NPN      TACH-3-X19   71 | PM7-T5CCP1
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class for slot type C2020_BASE2
//
// channel  typ             physical unit           WebBusTyp                Name          CpuPin|Port-Funct(t10-p-a2)                (new t10-p-a6)
//  0       analog in       -35,000°C-+90,000°C     WEBBUS_PIN_AIN_KTY1K     PTC1-X41(ANI1)   13 | PE2-AIN1
//  1       analog in       -35,000°C-+90,000°C     WEBBUS_PIN_AIN_KTY1K     PTC2-X43(ANI2)   14 | PE1-AIN2
//  2       analog in       -35,000°C-+90,000°C     WEBBUS_PIN_AIN_KTY1K     PTC3-X29(ANI4)   18 | PK0-AIN16
//  3       analog in       -35,000°C-+90,000°C     WEBBUS_PIN_AIN_KTY1K     PTC4-X44(ANI3)   15 | PE0-AIN3
//  4       analog in       0µA - 20000000µA        WEBBUS_PIN_AIN_I         I1-X5(ANI0)      12 | PE3-AIN0
//  5       analog in       0µA - 20000000µA        WEBBUS_PIN_AIN_I         I2-X42(ANI6)     20 | PK2-AIN18
//  6       analog in       0µA - 20000000µA        WEBBUS_PIN_AIN_I         I3-X5(ANI5)      19 | PK1-AIN17
//  7       analog in       0,000000V-60,000000V    WEBBUS_PIN_AIN_U         +V_MON           21 | PK3-AIN19
//  8       analog in       0µA - 11000000µA        WEBBUS_PIN_AIN_I2        IFAN1             1 | PD0-AIN15
//  9       analog in       0µA - 11000000µA        WEBBUS_PIN_AIN_I2        IFAN2           127 | PD6-AIN5
// 10       analog in       0µA - 5000000µA         WEBBUS_PIN_AIN_I2        IFAN3           128 | PD7-AIN4
// 11       n.c.
// 12       n.c.
// 13       analog out      0,000000V-10,000000V    WEBBUS_PIN_AOUT_PWM_U    U1-X6(PWM5)      77 | PM1-T2CCP1
// 14       analog out      0,000000V-10,000000V    WEBBUS_PIN_AOUT_PWM_U    U2-X7(PWM6)      78 | PM0-T2CCP0
// 15       analog out      0,00000%-100,00000%     WEBBUS_PIN_AOUT_PWM      PH.CUT(ZC-TF)    27 | PQ3-??? + 95 | PB0-???    (95 | PB0-T4CCP0 + 42 | PF0-M0PWM0)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int32_t driverGetValue(uint32_t Slot, uint32_t Channel)
{
    if(Slot == 0)
    {
        if(Channel <= 12)
        {
            return getDigitalOutputValue(Channel);
        }
        if(Channel >= 13 && Channel <= 15)
        {
            tPwmStructTab *pwm_temp = Pwm + (Channel - 13);
            return getPWMValue(pwm_temp);
        }
        else
        {
            return PINERR_NOTEXISTS;
        }
    }
    if(Slot == 1)
    {
        if(Channel <= 12)
        {
            return getDigitalInputValue(Channel);
        }
        else
        {
            tTimerStructTab *Timer_temp = Timer + (Channel - 13);
            return getTimerValue(Timer_temp);
        }
    }
    if(Slot == 2)
    {
        if(Channel <= 10)
        {
            return GetADCValue(Channel);
        }
        if(Channel >=13 && Channel <= 14)
        {
            tTimerPwmStructTab *TimerPwm_temp = TimerPwm + (Channel - 13);
            return getTimerPwmGetValue(TimerPwm_temp);
        }
        if(Channel == 15)
        {
            tZeroCrossPhaseCutStructPad *ZeroCrossPhaseCut_temp = ZeroCrossPhaseCut;
            return getDriverZeroCrossPhaseCutValue(ZeroCrossPhaseCut_temp);
        }
        if(Channel == 16)
        {
            tZeroCrossPhaseCutStructPad *ZeroCrossPhaseCut_temp = ZeroCrossPhaseCut;
            return getDriverZeroCrossPhaseCutHz1000(ZeroCrossPhaseCut_temp);
        }
        return PINERR_NOTEXISTS;
    }

    return PINERR_NOTEXISTS;
}

void driverSetValue(uint32_t Slot, uint32_t Channel, int32_t value)
{
    if(Slot == 0)
    {
        if(Channel <= 12)
        {
            setDigitalOutputValue(Channel, value);
        }
        if(Channel >= 13 && Channel <= 15)
        {
            tPwmStructTab *Pwm_temp = Pwm + (Channel - 13);
            setPwmValue(Pwm_temp, value);
        }
    }
    if(Slot == 1)
    {
        if(Channel <= 12)
        {
            setDigitalInputValue(Channel, value);
        }
        else
        {
            tTimerStructTab *Timer_temp = Timer + (Channel - 13);
            setTimerValue(Timer_temp, value);
        }
    }
    if(Slot == 2)
    {
        if(Channel >=13 && Channel <= 14)
        {
            tTimerPwmStructTab *TimerPwm_temp = TimerPwm + (Channel - 13);
            setTimerPwmGetValue(TimerPwm_temp, value);
        }
        if(Channel == 15)
        {
            tZeroCrossPhaseCutStructPad *ZeroCrossPhaseCut_temp = ZeroCrossPhaseCut;
            setDriverZeroCrossPhaseCutValue(ZeroCrossPhaseCut_temp, value);
        }
        if(Channel == 16)
        {
            tZeroCrossPhaseCutStructPad *ZeroCrossPhaseCut_temp = ZeroCrossPhaseCut;
            setDriverZeroCrossPhaseCutPulseWidth(ZeroCrossPhaseCut_temp, value);
        }
    }
}

int ValueGet(unsigned int Slot, unsigned int Channel)
{
    return driverGetValue(Slot, Channel);
}

void ValueSet(unsigned int Slot, unsigned int Channel, int value)
{
    driverSetValue(Slot, Channel, value);
}
