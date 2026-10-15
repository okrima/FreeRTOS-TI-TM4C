/*
 * multdiv.c
 *
 *  Created on: 30 mag 2024
 *      Author: albanese
 */

#include "stdint.h"
#include "multdiv.h"

uint32_t muldivu(uint32_t a,uint32_t b,uint32_t c)
{
    if(!a||!b)
        return 0;
    if(!c)
        return 0xFFFFFFFF;
    else
    {
        uint64_t x=((uint64_t)a * (uint64_t)b) / (uint64_t)c;
        if(x>(uint64_t)(0x00000000FFFFFFFFul))
            return 0xFFFFFFFF;
        return (uint32_t)x;
    }
}

int32_t muldiv(int32_t p1,int32_t p2,int32_t p3)
{
    unsigned char negativ;
    negativ=0;
    if(p1<0)
    {
        p1=-p1;
        negativ=!negativ;
    }
    if(p2<0)
    {
        p2=-p2;
        negativ=!negativ;
    }
    if(p3<0)
    {
        p3=-p3;
        negativ=!negativ;
    }
    p1=muldivu(p1,p2,p3);
    if(p1&0x80000000)
        return negativ ? 0x80000001 : 0x7FFFFFFF;
    return negativ?-p1:p1;
}



