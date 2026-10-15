/*
 * GestInOut.c
 *
 *  Created on: 2 lug 2024
 *      Author: albanese
 */

#include "GestInOut.h"
#include "App_global.h"
#include "AnInOut.h"

// Variabili relative alle risorse logiche
unsigned char Din[NDINLOG];          // Immagini degli INGRESSI DIGITALI LOGICI
unsigned char Dout[NDOUTLOG];        // Immagini delle USCITE DIGITALI LOGICHE
unsigned long Aout[NAOUTLOG];       // Immagini delle USCITE ANALOGICHE LOGICHE
signed long Ain[NAINLOG];         // Immagini degli INGRESSI ANALOGICI LOGICI
unsigned char StsDin[NDINLOG];       // Memorizza lo stato degli INGRESSI DIGITALI LOGICI
unsigned char StsDout[NDOUTLOG];     // Memorizza lo stato delle USCITE DIGITALI LOGICHE
unsigned char StsAout[NAOUTLOG];     // Memorizza lo stato degli USCITE ANALOGICHE LOGICHE
unsigned char StsAin[NAINLOG];       // Memorizza lo stato degli INGRESSI ANALOGICI LOGICI

// Inizializzazione delle risorse logiche
void IniOutputLogChannels(void);

char FirstTimeAnout = 1;

/**********************************************************************
NOME        : void IniOutputLogChannels(void)
DESCRIZIONE : Inizializzazione delle uscite digitali/analogiche logiche
ARGOMENTI   :
RETURN      :
**********************************************************************/
void IniOutputLogChannels(void)
{
    unsigned char i;

    for(i = 0; i<NAOUTLOG; i++)
    {
        //Aout[i] = 0;
    }
    for(i = 0; i<NDOUTLOG; i++)
    {
        //Dout[i] = OFF;
    }
}



/**********************************************************************
NOME        : void RefreshAnout(void)
DESCRIZIONE : Funzione che aggiorna lo stato delle uscite analogiche logiche e fisiche.
              NOTA: se piu' ANOUT-logici sono mappati sullo stesso ANOUT-fisico
                    allora viene assegnato il valore massimo.
ARGOMENTI   :
RETURN      :
**********************************************************************/
void RefreshAnout(void)
{
    int ch_index, ch_log_index;

    if(FirstTimeAnout == 1)
    {
        IniOutputLogChannels();
        FirstTimeAnout = 0;
    }

    //Aggiorno l'immagine fisica delle uscite analogiche
    for(ch_index = 0; ch_index<NAOUT; ch_index++)
    {
        ImgAout[ch_index] = ImgAoutLog[ch_index];
    }

    AnOutHdl();
}


/****************************** FUNZIONI *****************************/

/**********************************************************************
NOME        : void RefreshInput(void)
DESCRIZIONE : Refresh degli Ingressi (anoligici e digitali)
              ad intervalli di 200ms.
ARGOMENTI   :
RETURN      :
**********************************************************************/
void RefreshInput(void)
{
    RefreshDigin();
}

void RefreshOutput(void)
{
    RefreshAnout();

    RefreshDigOut();
}
