/*
 * utility_TM4C129X__.cpp
 *
 *  Created on: 6 giu 2024
 *      Author: albanese
 */

#include "stdint.h"

#include "utility_TM4C129X_.h"
#include "pindef__TM4C129X__.h"

void SetOBit(uint32_t bit, int32_t value)
{
    if(value)
        BitTableSetDisable(pO_Buffer,bit);
    else BitTableClrDisable(pO_Buffer,bit);
}
void SetIBit(uint32_t bit, int32_t value) {if(value) BitTableSetDisable(pI_Buffer,bit); else BitTableClrDisable(pI_Buffer,bit);}


//ROTE LED (HD / Flash zugriff )
void LedRedOff(void)         {GpioPortBitSet(L,4,0);}
void LedRedOn(void)          {GpioPortBitSet(L,4,1);}
void LedRedTgl(void)         {GpioPortBitTgl(L,4);}
void LedRedInit(void)        {GpioPortInitBitOut8mA(L,4,0);}

//ROTE LED (HD / Flash zugriff )
void LedGreenOff(void)       {GpioPortBitSet(L,5,0);}
void LedGreenOn(void)        {GpioPortBitSet(L,5,1);}
void LedGreenTgl(void)       {GpioPortBitTgl(L,5);}
void LedGreenInit(void)      {GpioPortInitBitOut8mA(L,5,0);}

// Led App
void LedAppOff(void)         {GpioPortBitSet(F,1,0);}
void LedAppOn(void)          {GpioPortBitSet(F,1,1);}
void LedAppTgl(void)         {GpioPortBitTgl(F,1);}
void LedAppInit(void)        {GpioPortInitBitOut8mA(F,1,0);}

uint32_t BitTableGet(const void *bittable, uint32_t pos)
{
    return ((((unsigned char *)bittable)[pos/8] & BIT(pos)) != 0);
}

void BitTableSetDisable(void *bittable, uint32_t pos)
{
    uint8_t *p=&((unsigned char *)bittable)[pos/8];
    uint8_t _or_ = BIT(pos);
    disable();
    *p |= _or_;
    enable();
}

void BitTableClrDisable(void *bittable, uint32_t pos)
{
    uint8_t *p=&((unsigned char *)bittable)[pos/8];
    uint8_t _and_ =~BIT(pos);
    disable();
    *p &= _and_;
    enable();
}


