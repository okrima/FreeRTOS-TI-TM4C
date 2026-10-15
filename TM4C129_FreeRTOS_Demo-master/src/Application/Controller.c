/*
 * Controller.c
 *
 *  Created on: 2 lug 2024
 *      Author: albanese
 */


#include "Controller.h"
#include "AnInOut.h"
#include "DigIOhdl.h"



void Controller(void)
{
/*  TEST ANALOG OUTPUT
 *
    ImgAoutLog[AOUT_DC1] = 50;
    ImgAoutLog[AOUT_DC2] = 75;
    ImgAoutLog[AOUT_DC3] = 25;

    ImgAoutLog[AOUT_AO1] = 25;
    ImgAoutLog[AOUT_AO2] = 75;
*
*/

//TEST DIGITAL INPUT TO DIGITAL OUTPUT
    char i;

    //Azzero le uscite
    ImgDigOut.vWORD = 0;

    for(i=0; i<NDIN; i++)
    {
        if(i >= AC_FAN_POWER)
            continue;

        if(ImgDigIn.vWORD & (1<<i))
            ImgDigOut.vWORD |= (1<<i);
    }

}

