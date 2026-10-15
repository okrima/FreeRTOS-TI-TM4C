/*
 * AnOut.h
 *
 *  Created on: 20 giu 2024
 *      Author: albanese
 */

#ifndef SRC_APPLICATION_ANOUT_H_
#define SRC_APPLICATION_ANOUT_H_

#define RESOLUTION_AOx  100000


enum chan {IC1,T1,T2,T3,T4,IC3,IC2,I_FAN1,V_MON,I_FAN2,I_FAN3,NCHANNEL};

// INGRESSI ANALOGICI MAPPABILI SU RISORSE LOGICHE (NAIN -> numero ingressi analogici mappabili su SB7  )
enum EnumAnin
{
    AIN_T1,     // ANIN0
    AIN_T2,     // ANIN1
    AIN_T3,     // ANIN2
    AIN_T4,     // ANIN3
    AIN_IC1,    // ANIN4
    AIN_IC2,    // ANIN5
    AIN_IC3,    // ANIN6

    NAIN
};

// INGRESSI ANALOGICI NON MAPPABILI SU RISORSE LOGICHE(NAINTOT -> numero totale ingressi analogici presenti)
enum EnumExtraAnin
{
    AIN_I_FAN1=NAIN,// ANIN7
    AIN_I_FAN2,     // ANIN8
    AIN_I_FAN3,     // ANIN9
    AIN_V_MON,      // ANIN10

    NAINTOT
};

// USCITE ANALOGICHE (NAOUT -> numero uscite analogiche)
enum EnumAnout
{
    AOUT_AO1,
    AOUT_AO2,
    AOUT_DC1,
    AOUT_DC2,
    AOUT_DC3,
    AOUT_AC,

    NAOUT
};

float ImgAin[NAINTOT];          // Immagine degli ingressi analogici acquisiti
int FilterCoeffSB7[NAINTOT];    // Memorizza i coefficieti dei filtri digitali su anin
unsigned char AinTypeSB7[NAINTOT];       // Tipo di canale su SB7 (0=PTC, 1=PT1000)
float ImgAout[NAOUT],           // Immagine fisica delle uscite analogiche
      ImgAoutLog[NAOUT];        // Immagine logica (senza tipo e range) delle uscite analogiche
unsigned char AoutRange[NAOUT];          // Range assegnato alle uscite analogiche (0=0-5V, 1=0-10V)
unsigned char AoutType[NAOUT];           // Tipo associato alle uscite analogiche (0=0-100%, 1=100-0%)

// Flags Indicanti se la sonda sbatte in alto
char Overflow;
// Flags Indicanti se la sonda sbatte in basso
char Underflow;
// valore A/D
unsigned short wValAdc[NCHANNEL];

float m[NCHANNEL], q[NCHANNEL];

void AnOutHdl();
void setAnalogOutput(char ch, char status_pwm);



#endif /* SRC_APPLICATION_ANOUT_H_ */
