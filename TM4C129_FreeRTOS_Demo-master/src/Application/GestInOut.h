/*
 * GestInOut.h
 *
 *  Created on: 2 lug 2024
 *      Author: albanese
 */

#ifndef SRC_APPLICATION_GESTINOUT_H_
#define SRC_APPLICATION_GESTINOUT_H_

#include "alarm.h"

// Valore che segnala quando lo stato logico di una risorsa logica digitale non e' disponibile e
// che quindi la risorsa fisica non deve essere aggiornata
#define NO_DISP         255

// Valore che segnala quando lo stato logico di una risorsa logica analogica non e' disponibile e
// che quindi la risorsa fisica non deve essere aggiornata
#define NO_DISP_AN      9999

// Codici di errore associati ai canali analogici logici (assegnati ad StsAin[])
#define CH_OK           0       // Canale gestito correttamente
#define CH_RANGE_ERR    1       // Canale fuori range
#define CH_INDEX_ERR    2       // Errore di configurazione del canale logico: indice del canale fisico errato
#define CH_NA           3       // Canale logico non assegnato a nessun canale fisico

// Valori usate per la gestione della polarita' dei digout
#define POL_DO_NO   0   // Polarity normally open
#define POL_DO_NC   1   // Polarity normally closed
#define POL_DO_NA   2   // Polarity not assigned
#define POL_DO_ERR  3   // Polarity error setting (i.e. conflict)

// Valori usate per la gestione del range degli anout
#define RANGE_AO_0_5    0   // range 0-5
#define RANGE_AO_0_10   1   // range 0-10
#define RANGE_AO_NA     2   // range not assigned
#define RANGE_AO_ERR    3   // range error setting (i.e. conflict)

// Valori usate per la gestione del tipo degli anout
#define TYPE_AO_DIR     0   // type direct (0-100%)
#define TYPE_AO_REV     1   // type reverse(100-0%)
#define TYPE_AO_NA      2   // type not assigned
#define TYPE_AO_ERR     3   // type error setting (i.e. conflict)

// Valori usate per la gestione del tipo degli anin
#define TYPE_AI_PTC     0   // type PTC
#define TYPE_AI_PT1000  1   // type PT1000
#define TYPE_AI_NA      2   // type not assigned
#define TYPE_AI_ERR     3   // type error setting (i.e. conflict)

/****************************** TYPEDEF ******************************/
//#define NAIN_TOT  139     // ADD MNS STUPCW20.00 megio come define dato che e' utilizzata in vari posti   // MOD ACA STUPCW23.00 #16517 punto 1.1 // MOD ACA STUPCW25.00 #17492
#define NAIN_TOT    146     // MOD MNS STUPCW30.00 #20035
#define NAOUT_TOT   79      // ADD MNS STUPCW20.00 megio come define dato che e' utilizzata in vari posti   // MOD ACA STUPCW24.00 #16750 punto 1.4 // MOD ACA STUPCW25.00 #17492

/****************************** ENUM *********************************/

// Enumerazione dell'immagine degli INGRESSI DIGITALI LOGICI
enum EnumDinLog
{
    // Ingressi usati dal controllo
    DI_EXT_EN,                                                          // Extern enable
    DI_REC1_EN, DI_REC2_EN,                                             // Total recovery enable 1,2
    DI_CP1_TH, DI_CP2_TH, DI_CP3_TH, DI_CP4_TH, DI_CP5_TH, DI_CP6_TH,   // Compressors thermic alarm (Cp1..Cp6)
    DI_HP1, DI_HP2,                                                     // High Pressure pressostat alarm (Circ1, Circ2)
    DI_LP1, DI_LP2,                                                     // Low Pressure pressostat alarm (Circ1, Circ2)
    DI_FAN1_TH, DI_FAN2_TH,                                             // Fan group 1,2 thermic alarm (Circ1, Circ2)
    DI_FAN_SHARE_TH,                                                    // Fan gruop share thermic alarm
    DI_FAN_FC_TH,                                                       // Fan freecooling thermic alarm
    DI_PU1_TH, DI_PU2_TH,                                               // Pump 1,2 thermic alarm
    DI_HTH_TH,                                                          // Pre-Heating electrical heater thermic
    DI_HTPU1_TH, DI_HTPU2_TH,                                           // Pre-Heating pump 1,2 thermic (NON USATE, visto che le pompe sono quelle del Cooling)
    DI_AFH_TH,                                                          // Antifreeze electrical heater thermic
    DI_AFPU1_TH, DI_AFPU2_TH,                                           // Antifreeze pump 1,2 thermic
    DI_EVAP_FLOW,                                                       // Eevaporator fluid flow
    DI_LEV_TANK,                                                        // Low level tank
    DI_LWL_PU1, DI_LWL_PU2,                                             // Low water level tank - pump 1,2  //{ADD MB STUPRC01.00: ECR3240 - Allarmi Low water level pump 1,2 //}
    DI_PHASE_ERR,                                                       // Wrong phase switch   //{ADD MB STUPRC02.00: ECR3298 - Allarme Wrong phase //}
    DI_CP1_OIL, DI_CP2_OIL,                                             // Compressors oil level alarm (Cp1,Cp2)    //{ADD MB STUPRC08.00: ECR4221 punto 4 - Allarmi Cp oil level //}
    DI_OFFSET,                                                          // DI per offset set point                  // ADD MNS STUPCW07.00
    DI_POWER_FAIL,                                                      // DI allarme power failure                 // ADD MNS STUPCW07.00
    DI_COND_PUMP1_TH,                                                   // DI allarme termico condenser pump 1      // ADD MNS STUPCW07.00
    DI_COND_PUMP2_TH,                                                   // DI allarme termico condenser pump 2      // ADD MNS STUPCW07.00
    DI_DRY_COOLER_TH,                                                   // DI allarme termico dry cooler            // ADD MNS STUPCW07.00
    DI_LIM_DEM,                                                         // DI per limit demand                      // ADD ALC STUPCW07.00 ECR 6016 p8
    DI_CFS,                                                             // DI per allarme Condenser flow switch     // ADD ALC STUPCW07.00 ECR 6016 p5
    DI_COND_SET,                                                        // DI per setpoint condensing pressure da DI    // ADD ALC STUPCW07.00 ECR 6016 p16
    DI_PU_TH_SC1,DI_PU_TH_SC2,DI_PU_TH_SC3,                             // DI per allarme termico pompe per circuiti secondari 1,2,3        // {ADD ALC STUPCW11.00 #10253
    DI_FLOW_SC1, DI_FLOW_SC2, DI_FLOW_SC3,                              // DI per allarme flusso per circuiti secondari 1,2,3               // }
    DI_DESUPH_HEAT1_TH, DI_DESUPH_HEAT2_TH,                             // DI per allarmi termici resistenze desuperheat    // ADD ACA STUPCW14.00 ECR #11150 : Aggiunti allarmi desuperheat heater thermic
    DI_USER_DEF1,                                                       // User defined 1 alarm     // {ADD ACA STUPCW15.00 ECR #11657 : Aggiunti allarmi definiti da utente
    DI_USER_DEF2,                                                       // User defined 2 alarm     // }
    DI_COND_PUMP_SH_TH,                                                 // Condenser pump share thermic // ADD ACA STUPCW24.00 #16750 punto 1
    // !! ADD MNS STUPCW08.00 mi avvantaggio su modifiche successive
    // DI_SET_OFFSET

    NDINLOG
};

// Enumerazione dell'immagine delle USCITE DIGITALI LOGICHE
enum EnumDoutLog
{
    // Uscite usate dal controllo
    DO_CP1_CNT1, DO_CP2_CNT1, DO_CP3_CNT1, DO_CP4_CNT1, DO_CP5_CNT1, DO_CP6_CNT1, // Contattore 1 compressori (Cp1..Cp6)    //{MOD MB STUPRC08.00: ECR4221 punto 5 - Controllo cp screw //}
    DO_LV1, DO_LV2,                                                     // Liquid valve 1,2 (Circ1, Circ2)
    DO_HEAT_REC1, DO_HEAT_REC2,                                         // Total recovery valve 1,2 (Circ1, Circ2)
    DO_PU1, DO_PU2,                                                     // Pump1,2
    DO_AF_HEAT,                                                         // Antifreeze heater
    DO_AF_PU1, DO_AF_PU2,                                               // Antifreeze pump 1,2
    DO_SYSTEM_ON,                                                       // System on relay
    DO_SYSTEM_OFF,                                                      // System off relay
    DO_SYSTEM_STDBY,                                                    // System stand-by relay
    DO_ALARM1, DO_ALARM2,                                               // General alarm relay 1,2
    DO_WARN1, DO_WARN2,                                                 // General warning relay 1,2
    DO_HT_HEAT,                                                         // Pre-heating heater
    DO_HT_PU1, DO_HT_PU2,                                               // Pre-heating pump 1,2 (NON USATE, visto che le pompe sono quelle del Cooling)
    DO_CP1_CNT2, DO_CP2_CNT2,                                           // Contattore 2 compressori (Cp1,Cp2)           //{ADD MB STUPRC08.00: ECR4221 punto 5 - Controllo cp screw //}
    DO_CP1_CR1, DO_CP2_CR1,                                             // Valvola CR1 su compressori screw (Cp1,Cp2)   //{ADD MB STUPRC08.00: ECR4221 punto 5 - Controllo cp screw //}
    DO_CP1_CR2, DO_CP2_CR2,                                             // Valvola CR2 su compressori screw (Cp1,Cp2)   //{ADD MB STUPRC08.00: ECR4221 punto 5 - Controllo cp screw //}
    DO_CP1_CR3, DO_CP2_CR3,                                             // Valvola CR3 su compressori screw (Cp1,Cp2)   //{ADD MB STUPRC08.00: ECR4221 punto 5 - Controllo cp screw //}
    DO_CP1_CR4, DO_CP2_CR4,                                             // Valvola CR4 su compressori screw (Cp1,Cp2)   //{ADD MB STUPRC08.00: ECR4221 punto 5 - Controllo cp screw //}
    DO_CP1_ECO, DO_CP2_ECO,                                             // Valvola Economizer su compressori screw (Cp1,Cp2)    //{ADD MB STUPRC08.00: ECR4221 punto 5 - Controllo cp screw //}
    DO_CP1_LIQINJ, DO_CP2_LIQINJ,                                       // Liquid Injection //  ADD MNS STUPCW04.00 ECR 4840 p.8.1
    DO_CP1_EQ_SOL, DO_CP2_EQ_SOL, DO_CP3_EQ_SOL, DO_CP4_EQ_SOL,         // Equalization Solenoid //{ADD ALC STUPCW4T.00 //}
    DO_CBV1, DO_CBV2,                                                   // Condenser Bypass valves  // ADD MNS STUPCW07.00 p.14
    DO_COND_PUMP1,DO_COND_PUMP2,                                        // Condenser pump           // ADD MNS STUPCW07.00 p.12
    DO_CRKHT,                                                           // crankcase heater         // ADD ALC STUPCW07.00 ECR 6016 p4
    DO_DESUH_AF1, DO_DESUH_AF2,                                         // Desuperheat Antifreeze heaters   //ADD ALC STUPCW10.00 ECR 6309 p3 Desuperheater antifreeze
    DO_HGBP1, DO_HGBP2,                                                 // Hot gas bypass valves            // {ADD ACA STUPCW11.00 #10253
    DO_PUMP_SC1,DO_PUMP_SC2,DO_PUMP_SC3,                                // Pump secondary circuits 1,2,3    // }
    DO_COOLING,                                                         // Cooling status                   //{ADD ACA STUPCW22.00 #15138
    DO_MIX_MODE,                                                        // Mix Mode status
    DO_FREECOOLING,                                                     // Freecooling status               //}
    DO_COOLING_DIS1,                                                    // Cooling disable group 1          //{ADD ACA STUPCW24.00 #16750 punto 2
    DO_COOLING_DIS2,                                                    // Cooling disable group 2          //}
    DO_COND_PUMP_SH,                                                    // Condenser pump share             //{ADD ACA STUPCW24.00 #16750 punto 1.4
    //(ATTENZIONE: mettere tutti i nuovi DO al di sopra di questa riga !!!)
    // Uscite associate agli allarmi
    DO_AL_OPERATOR1,                                                    // Alarms relay - operator 1
    DO_AL_OPERATOR2=DO_AL_OPERATOR1+A_NUM,                              // Alarms relay - operator 2

    NDOUTLOG=DO_AL_OPERATOR2+A_NUM,
};

// Enumerazione dell'immagine delle USCITE ANALOGICHE LOGICHE
enum EnumAoutLog
{
    AO_FAN1, AO_FAN2,           // Fan group 1,2 (Circ1,Circ2)
    AO_FAN_SHARE,               // Fan group share
    AO_FC_FAN,                  // Freecooling fan
    AO_FC_VALVE,                // Freecooling valve
    AO_EXPV1, AO_EXPV2,         // Electronic expansion valve 1,2 (Circ1, Circ2)
    EX_AO_CP1_DEM,              // Viene lasciato per compatibilita' user grafica                   // MOD MNS STUPCW4T.00 gestione turbocor multicompressore
    AO_CV1, AO_CV2,             // Condenser Valve group 1,2 (Circ1,Circ2)  // ADD MNS STUPCW4.00 ECR4840 p.10
    AO_CP1_DEM,                 // Compressor 1 demand request  //{ADD MB STUPRC1T.00: aggiunto controllo inverter dei cp //}   // MOD MNS STUPCW4T.00 gestione turbocor multicompressore
    AO_CP2_DEM,                 // ADD MNS STUPCW4T.00 gestione turbocor multicompressore
    AO_CP3_DEM,                 // ADD MNS STUPCW4T.00 gestione turbocor multicompressore
    AO_CP4_DEM,                 // ADD MNS STUPCW4T.00 gestione turbocor multicompressore
    AO_COND_PUMP1,              // ADD MNS STUPCW07.00 Condensenr pump 1
    AO_COND_PUMP2,              // ADD MNS STUPCW07.00 Condensenr pump 2
    AO_DRY_COOLER,              // ADD MNS STUPCW07.00 Dry cooler
    AO_HEAT_REC_VALVE1,         // ADD ALC STUPCW07.00 Heat recovery valve 1
    AO_HEAT_REC_VALVE2,         // ADD ALC STUPCW07.00 Heat recovery valve 2
    AO_HGBP1,                   // ADD ACA STUPCW11.00 #10253 Hot gas bypass valve 1
    AO_HGBP2,                   // ADD ACA STUPCW11.00 #10253 Hot gas bypass valve 2
    AO_COOLVALVE_SC1,           // ADD ACA STUPCW11.00 ECR 10253 Cooling valve secondary circuit 1
    AO_COOLVALVE_SC2,           // ADD ACA STUPCW11.00 ECR 10253 Cooling valve secondary circuit 2
    AO_COOLVALVE_SC3,           // ADD ACA STUPCW11.00 ECR 10253 Cooling valve secondary circuit 3
    AO_CV_SHARE,                // ADD ACA STUPCW23.00 #16517 punto 1.2 Condenser valve share
    AO_COND_PUMP_SHARE,         // ADD ACA STUPCW24.00 #16750 punto 1.4
    NAOUTLOG
};

// Enumerazione dell'immagine degli INGRESSI ANALOGICI LOGICI
enum EnumAinLog
{
    AI_LWT,                     // Leaving Water Temp. (outlet temp)    //{MOD MB STUPRC03.00: ECR3298 - modificato nome da WT a LWT //}
    AI_TIE,                     // Inlet evaporator Temp.
    AI_TAF,                     // Antifreeze Temp.
    AI_TAE1,                    // External Temp. 1
    AI_TAE2,                    // External Temp. 2
    AI_TAE,                     // External Temp. (canale logico derivato (combinazione di altri canali logici)
    AI_DP1, AI_DP2,             // Condenser Press. 1,2 (Circ1, Circ2) (i.e. Discharge press.)
    AI_SP1, AI_SP2,             // Evaporator Press. 1,2 (Circ1, Circ2) (i.e. Suction Press.)
    AI_ST1, AI_ST2,             // Suction Temp. (Circ1, Circ2)
    AI_DT1, AI_DT2,             // Discharge Temp. (Circ1, Circ2)
    AI_RWT,                     // Return Water Temp. (inlet temp)      //{ADD MB STUPRC03.00: ECR3298 - Aggiunta sonda RWT  //}
    AI_CST,                     // Chiller Saver Temp.                  //{ADD MB STUPRC07.00: ECR3904 punto 7 - Implementato Chiller Saver //}
    AI_CHT,                     // Compressor Housing Temp.                 //{ADD MNS STUPCW04.00 ECR 4840 p.8.2 //}
    AI_LT1,                     // Liquid Temperature 1                 //{ADD ALC STUPCW4T.00
    AI_LT2,                     // Liquid Temperature 2                 //}
    AI_LL1,                     // Liquid Level 1                       // ADD MNS STUPCW4T.00
    AI_LL2,                     // Liquid Level 2                       // ADD MNS STUPCW4T.00
    AI_FF,                      // Fluid flow                           // ADD MNS STUPCW07.00 ECR 6016 p10
    AI_LIMDEM,                  // Limit Demand                         // ADD ALC STUPCW07.00 ECR 6016 p8
    AI_ICT1,                    // Inlet Condenser Temperature 1        //{ADD ALC STUPCW07.00 ECR 6016 p5
    AI_OCT1,                    // Outlet Condenser Temperature 1
    AI_ICT2,                    // Inlet Condenser Temperature 2
    AI_OCT2,                    // Outlet Condenser Temperature 2
    AI_CF,                      // Condenser Flow                       //}
    AI_TD1,                     // Differential Temperature 1           //ADD ALC STUPCW10.00 ECR 6309 p1 Advanced Cooling
    AI_CHT2,                    // Compressor Housing temp. 2           // ADD ALC STUPCW10.00 ECR 6309 p4 Cold Start
    AI_CCT1,                    // Condenser Coil Temperature 1         //{ADD ALC STUPCW10.00 ECR 6309 p2 Partial Heat Recovery
    AI_CCT2,                    // Condenser Coil Temperature 2         //}
    AI_TSC1,                    // Temperature secondary circuit 1      //{ADD ACA STUPCW11.00 #10253
    AI_TSC2,                    // Temperature secpondary circuit 2
    AI_TSC3,                    // Temperature secondary circuit 3      //}
    AI_DPC,                     // Condenser pressure derivate          // ADD ACA STUPCW23.00 #16517 punto 1.1

    NAINLOG
};

extern char FirstTimeAnout;

extern unsigned char Din[];
unsigned char DigIN[];
extern unsigned char Dout[];
extern unsigned long Aout[];
extern signed long Ain[];
extern unsigned char StsDin[];
extern unsigned char StsDout[];
extern unsigned char StsAout[];
extern unsigned char StsAin[];

void RefreshAnoutLog(void);
void RefreshOutput(void);
void RefreshInput(void);


#endif /* SRC_APPLICATION_GESTINOUT_H_ */
