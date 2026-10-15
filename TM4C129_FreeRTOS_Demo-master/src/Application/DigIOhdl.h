#ifndef __DIGIOHDL_H__
#define __DIGIOHDL_H__

#include "TYPEDEFS.H"

#define DIGIN_1     ImgDigIn.BIT.D0
#define DIGIN_2     ImgDigIn.BIT.D1
#define DIGIN_3     ImgDigIn.BIT.D2
#define DIGIN_4     ImgDigIn.BIT.D3
#define DIGIN_5     ImgDigIn.BIT.D4
#define DIGIN_6     ImgDigIn.BIT.D5
#define DIGIN_7     ImgDigIn.BIT.D6
#define DIGIN_8     ImgDigIn.BIT.D7
#define DIGIN_9     ImgDigIn.BIT.D8
#define DIGIN_10    ImgDigIn.BIT.D9
#define DIGIN_11    ImgDigIn.BIT.DA
#define DIGIN_12    ImgDigIn.BIT.DB
#define DIGIN_13    ImgDigIn.BIT.DC

extern char FirstTimeDigIn;

enum EnumDigIn
{
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    A8,
    A9,
    A10,
    A11,
    A12,
    A13,
    AC_FAN_POWER,   //{ADD MB STUACS01.00: gestione ingressi digitali fittizi su SB7
    V1_FEEDBACK,
    V2_FEEDBACK,
    V3_FEEDBACK,    //}

    NDIN
};

enum EnumDigOut
{
    K1,
    K2,     // Non presente su SB7
    K3,
    K4,
    K5,
    K6,
    K7,
    K8,
    K9,
    K10,
    K11,
    K12,
    K13,

    NDOUT   //{MOD MB STUPRC00.00: nuova gestione risorse fisiche //}
};

extern union TBitWord ImgDigOut, ImgDigOutLog;
extern union TBitWord ImgDigIn;

void RefreshDigin(void);
void RefreshDigOut(void);

#endif  //__DIGIOHDL_H__
