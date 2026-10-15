/*
 * AnInOut.c
 *
 *  Created on: 20 giu 2024
 *      Author: albanese
 */


#include "AnInOut.h"
#include "App_pwm.h"
#include "App_global.h"

#include "driver.h"

const char BoardAdEnum[NCHANNEL]={0,4,2,1,3,5,6,7,3,4,5};

const char BoardAinEnum[NCHANNEL]=
{
    AIN_IC1,
    AIN_T1,
    AIN_T2,
    AIN_T3,
    AIN_T4,
    AIN_IC3,
    AIN_IC2,
    AIN_I_FAN1,
    AIN_V_MON,
    AIN_I_FAN2,
    AIN_I_FAN3,
};

// Tabelle per sonde PTC

const int p_ptc[16]=
{
    9484,
    10174,
    10951,
    11766,
    12585,
    13419,
    14251,
    15090,
    15934,
    16777,
    17609,
    18445,
    19257,
    20068,
    20861,
    21642
};

const float mspez_ptc[16]=
{
    0,
    0.014504507,
    0.01286042,
    0.012279559,
    0.012205455,
    0.01199536,
    0.012016284,
    0.01191109,
    0.011715256,
    0.011852793,
    0.012025835,
    0.011958036,
    0.012325008,
    0.012323477,
    0.012611932,
    0.012800022,
};

const float qspez_ptc[16] =
{
    0,
    -187.565764,
    -170.8391736,
    -164.4779545,
    -163.6060606,
    -160.9620081,
    -161.2427922,
    -159.7436732,
    -156.7884615,
    -158.8591463,
    -161.7623529,
    -160.5684848,
    -167.3373518,
    -167.3078725,
    -173.0965909,
    -177.0203234,
};

// Tabelle per sonde PT1000

const int r_pt1000[27]=
{
    562,        // -110°C
    603,        // -100°C
    643,        // -90°C
    683,        // -80°C
    723,        // -70°C
    763,        // -60°C
    803,        // -50°C
    843,        // -40°C
    882,        // -30°C
    922,        // -20°C
    961,        // -10°C
    1000,       // 0°C
    1039,       // 10°C
    1078,       // 20°C
    1117,       // 30°C
    1155,       // 40°C
    1194,       // 50°C
    1232,       // 60°C
    1271,       // 70°C
    1309,       // 80°C
    1347,       // 90°C
    1385,       // 100°C
    1423,       // 110°C
    1461,       // 120°C
    1498,       // 130°C
    1535,       // 140°C
    1573,       // 150°C
};

const float mspez_pt1000[27]=
{
    0.245098,   // -110°C
    0.2457,     // -100°C
    0.247525,   // -90°C
    0.248139,   // -80°C
    0.25,       // -70°C
    0.25,       // -60°C
    0.251256,   // -50°C
    0.252525,   // -40°C
    0.253165,   // -30°C
    0.253807,   // -20°C
    0.254453,   // -10°C
    0.255754,   // 0°C
    0.25641,    // 10°C
    0.257069,   // 20°C
    0.257732,   // 30°C
    0.258398,   // 40°C
    0.259067,   // 50°C
    0.260417,   // 60°C
    0.260417,   // 70°C
    0.26178,    // 80°C
    0.262467,   // 90°C
    0.263158,   // 100°C
    0.26455,    // 110°C
    0.26455,    // 120°C
    0.265957,   // 130°C
    0.266667,   // 140°C
    0.266667,   // 150°C
};

const float qspez_pt1000[27]=
{
    -247.721,   // -110°C
    -248.059,   // -100°C
    -249.158,   // -90°C
    -249.553,   // -80°C
    -250.825,   // -70°C
    -250.825,   // -60°C
    -251.784,   // -50°C
    -252.803,   // -40°C
    -253.3418,  // -30°C
    -253.9086,  // -20°C
    -254.5038,  // -10°C
    -255.7545,  // 0°C
    -256.4103,  // 10°C
    -257.0951,  // 20°C
    -257.8093,  // 30°C
    -258.553,   // 40°C
    -259.3264,  // 50°C
    -260.9375,  // 60°C
    -260.9375,  // 70°C
    -262.6702,  // 80°C
    -263.5696,  // 90°C
    -264.5,     // 100°C
    -266.4286,  // 110°C
    -266.4286,  // 120°C
    -268.484,   // 130°C
    -269.547,   // 140°C
    -269.547,   // 150°C
};



void ADConv(int canale , unsigned short wVal);
float calcola_temperatura_ptc(int par);
float calcola_temperatura_pt1000(int par);

void AnOutHdl(void);

void setAnalogOutput(char ch, char status_pwm)
{
    int Aux;

    switch (ch) {
        case AOUT_AC:   //ACFAN
            switch (status_pwm) {
                case 0:
                default:
                    Aux = ImgAout[AOUT_AC];
                    ValueSet(2, 16, (int)CfTriacPulseLen*1000);

                    //TODO: Controllo dei limiti

                    ValueSet(2, 15, Aux*100);
                    break;
            }
            break;
        case AOUT_AO1:  //ANOUT 1
            switch (status_pwm) {
                case 0:
                default:
                    Aux = ImgAout[AOUT_AO1];

                    ValueSet(2, 13, Aux*RESOLUTION_AOx);
                    break;
            }
            break;
        case AOUT_AO2:  //ANOUT 2
            switch (status_pwm) {
                case 0:
                default:
                    Aux = ImgAout[AOUT_AO2];

                    ValueSet(2, 14, Aux*RESOLUTION_AOx);
                    break;
            }
            break;
        case AOUT_DC1:  //DCFAN 1
            switch (status_pwm) {
                case 0:
                default:
                    Aux = ImgAout[AOUT_DC1];

                    ValueSet(0, 13, Aux*RESOLUTION_AOx);
                    break;
            }
            break;
        case AOUT_DC2:  //DCFAN 2
            switch (status_pwm) {
                case 0:
                default:
                    Aux = ImgAout[AOUT_DC2];

                    ValueSet(0, 14, Aux*RESOLUTION_AOx);
                    break;
            }
            break;
        case AOUT_DC3:  //DCFAN 3
            switch (status_pwm) {
                case 0:
                default:
                    Aux = ImgAout[AOUT_DC3];

                    ValueSet(0, 15, Aux*RESOLUTION_AOx);
                    break;
            }
            break;
        default:
            break;
    }
}

void AnOutHdl(void)
{
    unsigned char i = 0;
    for(i=0; i<NAOUT; i++)
    {
        setAnalogOutput(i, 0);
    }
}

