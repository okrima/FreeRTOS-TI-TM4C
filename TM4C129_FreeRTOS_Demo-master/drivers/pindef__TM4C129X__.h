
#ifndef __PINDEF__TM4C129X__H__
#define __PINDEF__TM4C129X__H__

#include "tm4c1294ncpdt.h"

#include "sysctl.h"
#include "gpio.h"
#include "hw_gpio.h"
#include "hw_types.h"
#include "pin_map.h"
#include "hw_memmap.h"


#ifdef __cplusplus
extern "C"
{
#endif


void BitTableClrDisable(void *bittable, uint32_t pos);
void BitTableSetDisable(void *bittable, uint32_t pos);
uint32_t BitTableGet(const void *bittable, uint32_t pos);

extern unsigned char pI_Buffer[120];
extern unsigned char pO_Buffer[120];

void SetOBit(uint32_t bit, int32_t value);

void SetIBit(uint32_t bit, int32_t value);

#define BIT(no) (1<<((no)&7))

#define GpioPort(port)                  (HWREG(GPIO_PORT##port##_BASE + (GPIO_O_DATA + ((0xFF) << 2))))
#define GpioPortBitGet(port,bit)        (HWREG(GPIO_PORT##port##_BASE + (GPIO_O_DATA + (1 << (2+(bit))))))
#define GpioPortBits(port,mask,values)  (HWREG(GPIO_PORT##port##_BASE + (GPIO_O_DATA + ((mask) << 2)))  = values)
#define GpioPortBitsTgl(port,mask)      (HWREG(GPIO_PORT##port##_BASE + (GPIO_O_DATA + ((mask) << 2))) ^= 0xFF)
#define GpioPortBitSet(port,bit,high)   (HWREG(GPIO_PORT##port##_BASE + (GPIO_O_DATA + (1 << (2+(bit)))))  = (high)?0xFF:0x00)
#define GpioPortBitTgl(port,bit)        (HWREG(GPIO_PORT##port##_BASE + (GPIO_O_DATA + (1 << (2+(bit))))) ^= 0xFF)
#define GpioPortInitBitsOut8mA(port,mask,InitLevels) ({                                         \
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO##port);                                       \
    GPIOPinTypeGPIOOutput(GPIO_PORT##port##_BASE, (mask));                                  \
    GPIOPadConfigSet(GPIO_PORT##port##_BASE,(mask),GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);        \
    GpioPortBits(port,(mask),(InitLevels)?0xFF:00);                                             \
})

#define GpioPortInitBitOut8mA(port,bit,high)     GpioPortInitBitsOut8mA(port,(1<<(bit)),(high)?0xFF:00)
#define GpioPortInitBitIn(port,bit)              do{SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO##port);GPIOPinTypeGPIOInput(GPIO_PORT##port##_BASE,1<<(bit));}while(0)


#define LED_DECLARE(name)     \
void name##Off(void);         \
void name##On(void);          \
void name##Tgl(void);         \
void name##Init(void);        \


#define LED_DEFINE(name,port,pin)       \
void name##Off(void)        {GpioPortBitSet(port,pin,1);}            \
void name##On(void)         {GpioPortBitSet(port,pin,0);}            \
void name##Tgl(void)        {GpioPortBitTgl(port,pin);}              \
void name##Init(void)       {GpioPortInitBitOut8mA(port,pin,1);}     \


//ROTE LED (HD / Flash zugriff )
void LedRedOff(void);
void LedRedOn(void);
void LedRedTgl(void);
void LedRedInit(void);

//ROTE LED (HD / Flash zugriff )
void LedGreenOff(void);
void LedGreenOn(void);
void LedGreenTgl(void);
void LedGreenInit(void);

// Led PE
void LedAppOff(void);
void LedAppOn(void);
void LedAppTgl(void);
void LedAppInit(void);

#define PioIn(Port,Pin)             GpioPortBitGet(Port,Pin)
#define PioInitIn(Port,Pin)         GpioPortInitBitIn(Port,Pin)
#define PioOut(Port,Pin,Level)      GpioPortBitSet(Port,Pin,Level)
#define PioInitOut(Port,Pin,Level)  GpioPortInitBitOut8mA(Port,Pin,Level)

#define GetOBit(bit)    BitTableGet(pO_Buffer,bit)
#define GetIBit(bit)    BitTableGet(pI_Buffer,bit)


#ifdef __cplusplus
}
#endif


//#endif // __TM4C129X__
#endif // __PINDEF__TM4C129X__H__
