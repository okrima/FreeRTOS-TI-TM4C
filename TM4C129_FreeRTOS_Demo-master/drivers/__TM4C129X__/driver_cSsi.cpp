#include "stdint.h"

#include "driver_cSsi.h"
#include "utility_TM4C129X_.h"

#include "pindef__TM4C129X__.h"

// SSI:  http://datasheet.digitronic.com/ARM/STELLARIS-TEXAS-INSTRUMENTS/TM4/TM4C1294_M4_1024Flash_256SRAM_ETH_MAC_PHY.pdf#page=1226

#define C2020_VERSION_A6    6
#define C2020_VERSION_A2    2
#define C2020_VERSION_A       C2020_VERSION_A6

#define Expansion_SYSCTL_PERIPH SYSCTL_PERIPH_SSI3
#define Expansion_Base SSI3_BASE
#define Expansion_DR_R SSI3_DR_R
#define Expansion_SR_R SSI3_SR_R
#if C2020_VERSION_A==C2020_VERSION_A2
#define Expansion_Select()          do{GpioPortBitSet(F,2,0);}while(0)
#define Expansion_Deselect()        do{GpioPortBitSet(F,2,1);wait_us(1);}while(0)
#define Expansion_SelectInit()      do{GpioPortInitBitOut8mA(F,2,1);}while(0)
#define Expansion_PeriphEnable()    do{ \
                                                SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);    \
                                                SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);    \
                                                SysCtlPeripheralEnable(SsiC2020Expansion_SYSCTL_PERIPH);    \
                                                GPIOPinTypeSSI(GPIO_PORTF_AHB_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3);   \
                                                GPIOPinConfigure(GPIO_PF3_SSI3CLK); \
                                                GPIOPinConfigure(GPIO_PF1_SSI3XDAT0);   \
                                                GPIOPinConfigure(GPIO_PF0_SSI3XDAT1);   \
                                            }while(0)
#elif C2020_VERSION_A==C2020_VERSION_A6
#define Expansion_Select()          do{GpioPortBitSet(Q,1,0);}while(0)
#define Expansion_Deselect()        do{GpioPortBitSet(Q,1,1);wait_us(1);}while(0)
#define Expansion_SelectInit()      do{GpioPortInitBitOut8mA(Q,1,1);}while(0)
#define Expansion_PeriphEnable()    do{ \
                                                SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);    \
                                                SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);    \
                                                SysCtlPeripheralEnable(Expansion_SYSCTL_PERIPH);    \
                                                GPIOPinTypeSSI(GPIO_PORTQ_BASE,GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3);   \
                                                GPIOPinConfigure(GPIO_PQ0_SSI3CLK); \
                                                GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);   \
                                                GPIOPinConfigure(GPIO_PQ3_SSI3XDAT1);   \
                                            }while(0)
#endif

void wait_us(unsigned int us)
{
    us*=2;
    while(us--)
    {
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
    }
}

void expansionTransfer(char *dst,const char *src, uint32_t len)
{
    int CountCheck = 100;
    uint32_t rxlen=len;
    Expansion_Select();
    while(rxlen && CountCheck)
    {
        CountCheck--;
        if(len && (Expansion_SR_R & SSI_SR_TNF))
        {
            Expansion_DR_R = *src++;
            len--;
        }
        if(Expansion_SR_R & SSI_SR_RNE)
        {
            *dst++ = Expansion_DR_R;
            rxlen--;
        }
    }
    Expansion_Deselect();
    if (!CountCheck)
    {
        // C2020Data.ErrorSPI++;
    }
}

void expansionSetSpeed(uint32_t Hz)
{
    SSIDisable              (Expansion_Base);
    SSIConfigSetExpClk      (Expansion_Base,CLOCK,SSI_FRF_MOTO_MODE_3,SSI_MODE_MASTER,Hz,8);  // 20MHZ / 200kHz
    SSIEnable               (Expansion_Base);
}

void driverExpansionInit(uint32_t Hz)
{
    Expansion_SelectInit();                                                                         // SPI CS init
    Expansion_PeriphEnable();                                                                       // SPI init
    expansionSetSpeed(Hz);
}
