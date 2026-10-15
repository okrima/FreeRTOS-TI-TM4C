/*
 * DigIOhdl.c
 *
 *  Created on: 2 lug 2024
 *      Author: albanese
 */

#include "DigIOhdl.h"
#include "driver.h"
#include "App_global.h"


union TBitWord ImgDigIn;
union TBitWord ImgDigOut, ImgDigOutLog;

union TBitWord tmpImgDigIn, precImgDigIn;

char FirstTimeDigIn = 1;

/***************************************************************************
* FUNZIONE  : UpdateDigIn(void)
* SCOPO     : Aggiorna la copia in Ram degli ingressi digitali
***************************************************************************/
void RefreshDigin(void)
{

    int i;


    if(FirstTimeDigIn)   //INITIALIZATION
    {
        ImgDigIn.BIT.D0 = (!ValueGet(1,0) ? 1:0);   //DIGIN1-X5     / A1
        ImgDigIn.BIT.D1 = (!ValueGet(1,1) ? 1:0);   //DIGIN2-X34    / A2
        ImgDigIn.BIT.D2 = (!ValueGet(1,2) ? 1:0);   //DIGIN3-X5     / A3
        ImgDigIn.BIT.D3 = (!ValueGet(1,3) ? 1:0);   //DIGIN4-X5     / A4
        ImgDigIn.BIT.D4 = (!ValueGet(1,4) ? 1:0);   //DIGIN5-X15    / A5
        ImgDigIn.BIT.D5 = (!ValueGet(1,5) ? 1:0);   //DIGIN6-X13    / A6
        ImgDigIn.BIT.D6 = (!ValueGet(1,6) ? 1:0);   //DIGIN7-X12    / A7
        ImgDigIn.BIT.D7 = (!ValueGet(1,7) ? 1:0);   //DIGIN8-X35    / A8
        ImgDigIn.BIT.D8 = (!ValueGet(1,10) ? 1:0);  //DIGIN9-X39    / A9
        ImgDigIn.BIT.D9 = (!ValueGet(1,11) ? 1:0);  //DIGIN10-X40   / A10
        ImgDigIn.BIT.DA = (!ValueGet(1,8) ? 1:0);   //DIGIN11-X37   / A11
        ImgDigIn.BIT.DB = (!ValueGet(1,9) ? 1:0);   //DIGIN12-X38   / A12
        ImgDigIn.BIT.DC = (!ValueGet(1,12) ? 1:0);  //DIGIN13-X36   / A13

        tmpImgDigIn = ImgDigIn;
        precImgDigIn = ImgDigIn;

        FirstTimeDigIn = 0;
    }

    tmpImgDigIn.BIT.D0 = (!ValueGet(1,0) ? 1:0);
    tmpImgDigIn.BIT.D1 = (!ValueGet(1,1) ? 1:0);
    tmpImgDigIn.BIT.D2 = (!ValueGet(1,2) ? 1:0);
    tmpImgDigIn.BIT.D3 = (!ValueGet(1,3) ? 1:0);
    tmpImgDigIn.BIT.D4 = (!ValueGet(1,4) ? 1:0);
    tmpImgDigIn.BIT.D5 = (!ValueGet(1,5) ? 1:0);
    tmpImgDigIn.BIT.D6 = (!ValueGet(1,6) ? 1:0);
    tmpImgDigIn.BIT.D7 = (!ValueGet(1,7) ? 1:0);
    tmpImgDigIn.BIT.D8 = (!ValueGet(1,10) ? 1:0);
    tmpImgDigIn.BIT.D9 = (!ValueGet(1,11) ? 1:0);
    tmpImgDigIn.BIT.DA = (!ValueGet(1,8) ? 1:0);
    tmpImgDigIn.BIT.DB = (!ValueGet(1,9) ? 1:0);
    tmpImgDigIn.BIT.DC = (!ValueGet(1,12) ? 1:0);



    for(i=0; i<13; i++)
    {
        if( (tmpImgDigIn.vWORD & (BIT0<<i)) == (precImgDigIn.vWORD & (BIT0<<i)) )
        {
            //Campioni uguali, aggiorno lo stato degli ingressi digitali
            if(tmpImgDigIn.vWORD & (BIT0<<i))
            {
                ImgDigIn.vWORD |= (BIT0<<i);
            }
            else
            {
                ImgDigIn.vWORD &= ~(BIT0<<i);
            }
        }

    }

    precImgDigIn = tmpImgDigIn;


}

void SwitchDigOut(int DigOut, char OnOff)
{
    switch(DigOut)
    {
        case K1:
            ValueSet(0, 0, OnOff);
            break;
        case K3:
            ValueSet(0, 2, OnOff);
            break;
        case K4:
            ValueSet(0, 3, OnOff);
            break;
        case K5:
            ValueSet(0, 4, OnOff);
            break;
        case K6:
            ValueSet(0, 5, OnOff);
            break;
        case K7:
            ValueSet(0, 6, OnOff);
            break;
        case K8:
            ValueSet(0, 7, OnOff);
            break;
        case K9:
            ValueSet(0, 8, OnOff);
            break;
        case K10:
            ValueSet(0, 9, OnOff);
            break;
        case K11:
            ValueSet(0, 10, OnOff);
            break;
        case K12:
            ValueSet(0, 11, OnOff);
            break;
        case K13:
            ValueSet(0, 12, OnOff);
            break;
    }
}

void RefreshDigOut(void)
{
    char i;

    for(i=0; i<NDOUT; i++)
    {
        if(ImgDigOut.vWORD & (1<<i))
            SwitchDigOut(i,1);
        else
            SwitchDigOut(i,0);
    }
}


