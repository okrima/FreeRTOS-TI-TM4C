/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// driver for digital input and output
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pindef__TM4C129X__.h"
#include "driver_Dig.h"
#include "FreeRTOS.h"
#include "task.h"

unsigned char pI_Buffer[120];
unsigned char pO_Buffer[120];

// PIO:  http://datasheet.digitronic.com/ARM/STELLARIS-TEXAS-INSTRUMENTS/TM4/TM4C1294_M4_1024Flash_256SRAM_ETH_MAC_PHY.pdf#page=742

void driverTM4C129X_DigiOut_Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    GpioPortInitBitOut8mA(D,1,0);                               // Relay 1
    GpioPortInitBitOut8mA(P,4,0);                               // DBUS TX (before unused Relay 2)
    GpioPortInitBitOut8mA(M,2,0);                               // Relay 3
    GpioPortInitBitOut8mA(M,3,0);                               // Relay 4
    GpioPortInitBitOut8mA(M,4,0);                               // Relay 5
    GpioPortInitBitOut8mA(M,5,0);                               // Relay 6
    GpioPortInitBitOut8mA(M,6,0);                               // Relay 7
    GpioPortInitBitOut8mA(L,0,0);                               // Relay 8
    GpioPortInitBitOut8mA(L,1,0);                               // Relay 9
    GpioPortInitBitOut8mA(L,2,0);                               // Relay 10
    GpioPortInitBitOut8mA(L,3,0);                               // Relay 11
    GpioPortInitBitOut8mA(L,6,0);                               // Relay 12
    GpioPortInitBitOut8mA(L,7,0);                               // Relay 13
}

void driverTM4C129X_DigiIn_Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    GPIOPinTypeGPIOInput(GPIO_PORTP_BASE,GPIO_PIN_5);       // DBUS RX
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_2);       // digital input 1
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_3);       // digital input 2
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_4);       // digital input 3
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_5);       // digital input 4
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_6);       // digital input 5
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_7);       // digital input 6
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_2);       // digital input 7
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_3);       // digital input 8
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE,GPIO_PIN_1);       // digital input 9
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE,GPIO_PIN_2);       // digital input 10
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE,GPIO_PIN_3);       // digital input 11
    GPIOPinTypeGPIOInput(GPIO_PORTK_BASE,GPIO_PIN_6);       // digital input 12
    GPIOPinTypeGPIOInput(GPIO_PORTK_BASE,GPIO_PIN_7);       // digital input 13
}

void driverTM4C129X_DigiOut_Transfer()
{
    GpioPortBits(D,0x02,pO_Buffer[0]<<1);                       // Relay     1: pO_Buffer[0].0:0 -> D1
    GpioPortBits(P,0x10,(~pO_Buffer[0])<<3);                    // DBUS TX(R2): pO_Buffer[0].1:1 -> P4
    GpioPortBits(M,0x7C,pO_Buffer[0]   );                       // Relay  3- 7: pO_Buffer[0].2:6 -> M6-M2
    GpioPortBits(L,0x01,pO_Buffer[0]>>7);                       // Relay     8: pO_Buffer[0].7:7 -> L0
    GpioPortBits(L,0x0E,pO_Buffer[1]<<1);                       // Relay  9-11: pO_Buffer[1].0:2 -> L1-L3
    GpioPortBits(L,0xC0,pO_Buffer[1]<<3);                       // Relay 12-13: pO_Buffer[1].3:4 -> L6-L7
}

void driverTM4C129X_DigiIn_Transfer()
{
    TickType_t in;
    in =pO_Buffer[0] & 0xFD;                                    // mirror relay 1,3-8 output to input
    in|=(((~GpioPort(P))&0x20)>>4);                             // DBUS RX (R2): P5 -> pI_Buffer[0].1
    pI_Buffer[0]=in;                                            // mirror relay 1,3-8 and DBUS RX to input
    pI_Buffer[1]=pO_Buffer[1];                                  // mirror relay 9-12 output to input -> pI_Buffer[1]
    in =((GpioPort(D)&0x0C)>>2);                                // digital input  1- 2: D2-3 -> pI_Buffer[2].0:1
    in|=((GpioPort(C)&0xF0)>>2);                                // digital input  3- 6: C4-7 -> pI_Buffer[2].2:5
    in|=((GpioPort(F)&0x0C)<<4);                                // digital input  7- 8: F2-3 -> pI_Buffer[2].6:7
    pI_Buffer[2]=in;                                            // digital input  1- 8: to pI_Buffer[2]
    in =((GpioPort(H)&0x0E)>>1);                                // digital input  9-11: H1-3 -> pI_Buffer[3].0:2
    in|=((GpioPort(K)&0xC0)>>3);                                // digital input 12-13: K6-7 -> pI_Buffer[3].3:4
    pI_Buffer[3]=in;                                            // digital input  9-13: to pI_Buffer[3]
}

void HwDigTicker(void *pvParameters)
{
    for( ; ;)
    {
        driverTM4C129X_DigiIn_Transfer();
        driverTM4C129X_DigiOut_Transfer();
        vTaskDelay(1);
    }
}

void setDigitalOutputValue(uint32_t Channel, int32_t value)
{
    SetOBit(Channel, value);
}

int32_t getDigitalInputValue(uint32_t Channel)
{
    Channel = Channel + 16; //Questo perché pI_Buffer è diviso in 4 slot e i DIGIN sono nello slot n°3
    return GetIBit(Channel);
}

void setDigitalInputValue(uint32_t Channel, int32_t value)
{
    Channel = Channel + 16;
    SetIBit(Channel, value);
}


int32_t getDigitalOutputValue(uint32_t Channel)
{
    return GetOBit(Channel);
}

void driver_Dig_init()
{
    driverTM4C129X_DigiOut_Init();
    driverTM4C129X_DigiIn_Init();
}


