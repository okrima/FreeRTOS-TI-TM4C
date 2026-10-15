/*
 * driver_ZeroCrossPhaseCut.h
 *
 *  Created on: 04.06.2024
 *      Author: Mirko Albanese
 */

#ifndef __driver_ZeroCrossPhaseCut__
#define __driver_ZeroCrossPhaseCut__

#define NUMER_OF_ZERO_CROSS_PHASE_CUT   1

#define ZEROCROSS_TIMEOUT                   50000   // MNS era // timeout for zerocross 15ms
#define PHASECUT_PULSE_WIDTH_DEFAULT        1000    // pulsewith 1000 µs

#define Sb7AcFanPowFreq     CfPowFreq

typedef struct
{
    struct
    {
        uint32_t PeripheralEnablePort;
        uint32_t PeripheralEnableTimer;
        uint32_t PinConfig;
        uint32_t Pins;
        uint32_t ui32PortBase;
        uint32_t ui32TimerBase;
        uint32_t ui32Timer;
    }Capture;
    struct
    {
        uint32_t PeripheralEnablePort;
        uint32_t ui32PortBase;
        uint32_t Pins;
        uint32_t PeripheralEnablePwm;
        uint32_t PinConfig;
        uint32_t ui32PwmBase;
        uint32_t PwmGen;
        uint32_t PwmOut;
    }Pwm;
}tCaptureConfigTab;

typedef struct
{
    uint32_t PwmOInvBase;
    uint32_t PwmEnableBase;
    uint32_t PwmEnableBit;
    uint32_t PwmxCmpBase;
    uint32_t PwmxLoadBase;
    uint32_t PwmxCountBase;
    uint32_t PwmPins;
    uint32_t CaptureNow;
    uint32_t Captures[4];
    uint8_t CaptureLastInInx;
    uint8_t WasGood;
    uint8_t  padding_1;
    uint8_t Enable;
    uint32_t ms_behind_zerocross;         // MOD MNS era u8
    volatile uint32_t dClock;                         // measuered CPU clocks between 4 half-waves
    uint32_t TimerCaptureBase;
    uint32_t TimerValueBase;
    uint32_t PulseWidth;   // pulse width in µs, can be set from application using setPulseWidth
    uint32_t Offset;                                  // pulse offset in CPU clocks
    int32_t value;
}tZeroCrossPhaseCutStructPad;

extern tZeroCrossPhaseCutStructPad ZeroCrossPhaseCut[NUMER_OF_ZERO_CROSS_PHASE_CUT];

#ifdef __cplusplus
extern "C"
{
#endif

void setDriverZeroCrossPhaseCutInvert(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp, uint32_t invert);
uint32_t getDriverZeroCrossPhaseCutInvert(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp);
int32_t getDriverZeroCrossPhaseCutValue(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp);
void setDriverZeroCrossPhaseCutPulseWidth(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp, int32_t value);
void setDriverZeroCrossPhaseCutValue(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp, int32_t value);
int32_t getDriverZeroCrossPhaseCutHz1000(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp);

void HwDriverZeroCrossPhaseCutTicker(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp);
void HwDriverZCPCTicker(void *pvParameters);

void driverZeroCrossPWM_init(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp, const tCaptureConfigTab* pT);
void driverZeroCrossPhaseCut(tZeroCrossPhaseCutStructPad * ZeroCrossPhaseCutTemp);

void driver_zcpc();

#ifdef __cplusplus
}
#endif

#endif // __driver_ZeroCrossPhaseCut__
