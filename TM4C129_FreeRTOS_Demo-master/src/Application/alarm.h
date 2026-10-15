/*
 * alarm.h
 *
 *  Created on: 2 lug 2024
 *      Author: albanese
 */

#ifndef SRC_APPLICATION_ALARM_H_
#define SRC_APPLICATION_ALARM_H_


enum EnumAlarms
{
    AL_CP1_TH,          // Allarmi termici Cp1..6
    AL_CP2_TH,
    AL_CP3_TH,
    AL_CP4_TH,
    AL_CP5_TH,
    AL_CP6_TH,
    AL_FAN1_TH,         // Allarmi termici Fan group 1,2
    AL_FAN2_TH,
    AL_FANS_TH,         // Allarmi termico Fan group share
    AL_PU1_TH,          // Allarmi termici Pump 1,2
    AL_PU2_TH,
    AL_EEV1_WIN,        // Allarmi winding Eev 1,2
    AL_EEV2_WIN,
    AL_EEV1_COM,        // Allarmi comunicazione seriale Eev 1,2
    AL_EEV2_COM,
    AL_FCFAN_TH,        // Allarmi termico Fans freecooling
    AL_HTH_TH,          // Allarme termico Pre-Heating Electrical Heater
    AL_HTPU1_TH,        // Allarmi termici Pre-Heating Pump 1,2 (NOTA: Allarmi non usati visto che le pompe sono quelle del cooling)
    AL_HTPU2_TH,
    AL_AFH_TH,          // Allarme termico Antifreeze Electrical Heater
    AL_AFPU1_TH,        // Allarmi termici Antifreeze Pump 1,2
    AL_AFPU2_TH,
    AL_HP1,             // Allarmi high press. switch 1,2
    AL_HP2,
    AL_LP1,             // Allarmi low press. switch 1,2
    AL_LP2,
    AL_HDT1,            // Allarmi high discharge temp. 1,2
    AL_HDT2,
    AL_HT,              // Allarme high temp.
    AL_LT,              // Allarme low temp.
    AL_MAXT,            // Allarme max temp.
    AL_MINT,            // Allarme min temp.
    AL_AF1,             // Allarmi antifreeze temp. 1,2
    AL_AF2,
    AL_EVAP_FLOW,       // Allarme evaporator fluid flow
    AL_LEV_TANK,        // Allarme low level tank
    AL_LWT,             // Allarme sonda Leaving Water Temp. (outlet temp)  //{MOD MB STUPRC03.00: ECR3298 - modificato nome da WT a LWT //}
    AL_TIE,             // Allarme sonda Inlet evaporator Temp.
    AL_TAF,             // Allarme sonda Antifreeze Temp.
    AL_TAE1,            // Allarmi sonde External Temp. 1,2
    AL_TAE2,
    AL_DP1,             // Allarmi sonde Condenser Press. 1,2
    AL_DP2,
    AL_SP1,             // Allarmi sonde Evaporator Press. 1,2
    AL_SP2,
    AL_ST1,             // Allarmi sonde Suction Temp. 1,2
    AL_ST2,
    AL_DT1,             // Allarmi sonde Discharge Temp. 1,2
    AL_DT2,
    AL_S13_COMM,        // Allarme comunicazione con S13
    AL_S13_SW_VER,      // Allarme versione sw su S13
    AL_S13_SETUP_ERR,   // Allarme errore trasferimento setup su S13
    AL_LWL_PU1,         // Allarme low level tank - pump 1,2        //{ADD MB STUPRC01.00: ECR3240 - Allarmi Low water level pump 1,2
    AL_LWL_PU2,                                                     //}
    AL_PHASE_WRONG,     // Allarme wrong phase                      //{ADD MB STUPRC02.00: ECR3298 - Allarme Wrong phase //}
    AL_INV1_ENVELOPE,   // Allarme inverter 1 envelope              //{ADD MB STUPRC1T.00: aggiunto controllo inviluppo dei cp  //}
    AL_NOT_USED3,       // Allarme inverter 1 communication error   //{ADD MB STUPRC1T.00: aggiunto controllo inverter dei cp   //}
    AL_NOT_USED1,       // Allarme inverter 1 fault                 // MOD MNS STUPCW4T.00 non piu' usato si usa CP_FAULT1
    AL_HI_DELTAP_START1,// Allarme High Delta-P at startup 1,2      //{ADD MB STUPRC1T.00: aggiunto controllo inverter dei cp   //}
    AL_HI_DELTAP_START2,                                            //{ADD MB STUPRC1T.00: aggiunto controllo inverter dei cp   //}
    AL_LO_DELTAP_INCR1, // Allarme Low increase Delta-P after startup 1,2   //{ADD MB STUPRC1T.00: aggiunto controllo inverter dei cp   //}
    AL_LO_DELTAP_INCR2,                                                     //{ADD MB STUPRC1T.00: aggiunto controllo inverter dei cp   //}
    AL_LO_DELTAP1,      // Allarme Low Delta-P 1,2                  //{ADD MB STUPRC1T.00: aggiunto controllo inverter dei cp   //}
    AL_LO_DELTAP2,                                                  //{ADD MB STUPRC1T.00: aggiunto controllo inverter dei cp   //}
    AL_LOW_SP1,         // Allarme Low Suction Press. 1,2           //{ADD MB STUPRC03.00: aggiunto allarme Low Suction Press.  //}
    AL_LOW_SP2,                                                     //{ADD MB STUPRC03.00: aggiunto allarme Low Suction Press.  //}
    AL_RWT,             // Allarme sonda Return Water Temp. (inlet temp)    //{ADD MB STUPRC03.00: ECR3298 - Aggiunta sonda RWT  //}
    AL_HI_DIFF_TEMP,    // Allarme high differential temp.          //{ADD MB STUPRC03.00: ECR3584 - Aggiunti allarmi "High/Low differential temp. //}
    AL_LO_DIFF_TEMP,    // Allarme high differential temp.          //{ADD MB STUPRC03.00: ECR3584 - Aggiunti allarmi "High/Low differential temp. //}
    AL_SEQ_COM,         // Allarme sequencing serial communication  //{ADD MB STUPRC06.00: ECR3819 - Implementato sequencing //}
    AL_CST,             // Allarme sonda Chiller Saver Temp.        //{ADD MB STUPRC07.00: ECR3904 punto 7 - Implementato Chiller Saver //}
    AL_NOT_USED2,   // Allarme inverter 1 "warning"             //ADD MF STUPRC07.00 - ECR3904 punto 12 // NON USATO
    AL_INV1_EVENT,      // Allarme inverter 1 "event"               //ADD MF STUPRC07.00 - ECR3904 punto 12
    AL_PU1_FAULT,       // Allarmi Pump 1,2 fault                   //{ADD MB STUPRC07.00: ECR3904 punto 3 //}
    AL_PU2_FAULT,                                                   //{ADD MB STUPRC07.00: ECR3904 punto 3 //}
    AL_CP1_OIL,         // Allarmi livello olio Cp1,2               //{ADD MB STUPRC08.00: ECR4221 punto 4 - Allarmi Cp oil level //}
    AL_CP2_OIL,                                                     //{ADD MB STUPRC08.00: ECR4221 punto 4 - Allarmi Cp oil level //}
    AL_CP1_ENVELOPE,    // Allarmi inviluppo Cp1,2                  //{ADD MB STUPRC08.00: ECR4221 punto 5 - Controllo cp screw //}
    AL_CP2_ENVELOPE,                                                //{ADD MB STUPRC08.00: ECR4221 punto 5 - Controllo cp screw //}
    AL_CHT,             // Allarme sonda CH                         //{ADD MNS STUPCW04.00 ECR 4840 p.8.2 //}
    AL_POWER_ON,        // Allarme Power on                         // ADD MNS STUPCW05.00
//{ADD MNS STUPCW4T.00
    AL_CP_FAULT1,   // Allarme Turbocor Fault 1
    AL_CP_FAULT2,   // Allarme Turbocor Fault 2     // come parametri di setup si usa quelli del cp1
    AL_CP_FAULT3,   // Allarme Turbocor Fault 3     // come parametri di setup si usa quelli del cp1
    AL_CP_FAULT4,   // Allarme Turbocor Fault 4     // come parametri di setup si usa quelli del cp1
    AL_TCOR_BEAR1,  // Allarme Turbocor Bearing 1
    AL_TCOR_BEAR2,  // Allarme Turbocor Bearing 2   // come parametri di setup si usa quelli del cp1
    AL_TCOR_BEAR3,  // Allarme Turbocor Bearing 3   // come parametri di setup si usa quelli del cp1
    AL_TCOR_BEAR4,  // Allarme Turbocor Bearing 4   // come parametri di setup si usa quelli del cp1
    AL_TCOR_MOTOR1, // Allarme Turbocor Motor 1
    AL_TCOR_MOTOR2, // Allarme Turbocor Motor 2     // come parametri di setup si usa quelli del cp1
    AL_TCOR_MOTOR3, // Allarme Turbocor Motor 3     // come parametri di setup si usa quelli del cp1
    AL_TCOR_MOTOR4, // Allarme Turbocor Motor 4     // come parametri di setup si usa quelli del cp1
    AL_CP_WARNING1, // Allarme Turbocor WARNING 1
    AL_CP_WARNING2, // Allarme Turbocor WARNING 2   // come parametri di setup si usa quelli del cp1
    AL_CP_WARNING3, // Allarme Turbocor WARNING 3   // come parametri di setup si usa quelli del cp1
    AL_CP_WARNING4, // Allarme Turbocor WARNING 4   // come parametri di setup si usa quelli del cp1
    AL_CP_COMMUNICATION1,   // Allarme Turbocor COMMUNICATION 1
    AL_CP_COMMUNICATION2,   // Allarme Turbocor COMMUNICATION 2 // come parametri di setup si usa quelli del cp1
    AL_CP_COMMUNICATION3,   // Allarme Turbocor COMMUNICATION 3 // come parametri di setup si usa quelli del cp1
    AL_CP_COMMUNICATION4,   // Allarme Turbocor COMMUNICATION 4 // come parametri di setup si usa quelli del cp1
    AL_LT1,             // Allarme Liquid Temperature 1
    AL_LT2,             // Allarme Liquid Temperature 2

    AL_SETUPUPDATE,             // Setup update
    AL_LL1,             // Allarme Liquid level 1
    AL_LL2,             // Allarme Liquid level 2
//}
    AL_FF,              // Allarme fluid flow           // ADD MNS STUPCW07.00 ECR 6016
    AL_LFF,             // Allarme Low fluid flow       // ADD MNS STUPCW07.00 ECR 6016
    AL_POWER_FAIL,      // Allarme power failure        // ADD MNS STUPCW07.00 ECR 6016
    AL_COND_PUMP1,      // Allarme condenser pump 1     // ADD MNS STUPCW07.00 ECR 6016
    AL_COND_PUMP2,      // Allarme condenser pump 2     // ADD MNS STUPCW07.00 ECR 6016
    AL_DRY_COOLER,      // Allarme dry cooler           // ADD MNS STUPCW07.00 ECR 6016
    AL_LIMDEM1,         // Allarme in caso di limit demand del compressore 1    // {ADD ALC STUPCW07.00 ECR 6016 p8
    AL_LIMDEM2,         // Allarme in caso di limit demand del compressore 2    // }
    AL_STUHMIERROR,     // Allarme nel caso in cui non dovesse trovare il file symbol.csv nella cartella STUHMI_CUSTOM
    AL_SET_FILE_MISSING,        // set file mancante
    AL_SET_FILE_WRONG_VERSION,      // set file mancante
    AL_ICT1,            // Allarme sonda inlet condenser temperature 1 (ICT1)   // {ADD ALC STUPCW07.00 ECR 6016 p5
    AL_OCT1,            // Allarme sonda outlet condenser temperature 1 (OCT1)
    AL_ICT2,            // Allarme sonda inlet condenser temperature 2 (ICT2)
    AL_OCT2,            // Allarme sonda outlet condenser temperature 2 (OCT2)
    AL_LCF,             // Allarme Low condenser flow
    AL_CF,              // Allarme sonda condenser flow
    AL_CFS,             // Allarme condenser flow switch                        // }
    AL_EM_COMMUNICATION,// Allarme comunicazione Modbus con Energy Meter        // ADD ALC STUPCW08.00 ECR 6016 Comunicazione Modbus con Energy Meter
    AL_CHT2,            // Allarme sonda Cp Housing Temperature 2 (CHT2)        // ADD ALC STUPCW10.00 ECR 6309 p4 Cold Start - Aggiunta sonda CHT2
    AL_CCT1,            // Allarme sonda Condenser Coil temperature 1 (CCT1)    //{ADD ALC STUPCW10.00 ECR 6309 p2 Partial Heat Recovery - Aggiunte sonde CCT1 e CCT2
    AL_CCT2,            // Allarme sonda Condenser Coil temperature 2 (CCT2)    //}
    AL_PU_TH_SC1,       // Allarme termico pompa del circuito secondario 1      //{ADD ACA STUPCW11.00 #10253
    AL_PU_TH_SC2,       // Allarme termico pompa del circuito secondario 2
    AL_PU_TH_SC3,       // Allarme termico pompa del circuito secondario 3
    AL_FLOW_SC1,        // Allarme flusso del circuito secondario 1
    AL_FLOW_SC2,        // Allarme flusso del circuito secondario 2
    AL_FLOW_SC3,        // Allarme flusso del circuito secondario 3
    AL_HT_SC1,          // Allarme high temperature del circuito secondario 1
    AL_HT_SC2,          // Allarme high temperature del circuito secondario 2
    AL_HT_SC3,          // Allarme high temperature del circuito secondario 3
    AL_LT_SC1,          // Allarme low temperature del circuito secondario 1
    AL_LT_SC2,          // Allarme low temperature del circuito secondario 2
    AL_LT_SC3,          // Allarme low temperature del circuito secondario 3
    AL_HI_DIFF_TEMP_SC1,// Allarme high diff temperature del circuito secondario 1
    AL_HI_DIFF_TEMP_SC2,// Allarme high diff temperature del circuito secondario 2
    AL_HI_DIFF_TEMP_SC3,// Allarme high diff temperature del circuito secondario 3
    AL_LO_DIFF_TEMP_SC1,// Allarme low diff temperature del circuito secondario 1
    AL_LO_DIFF_TEMP_SC2,// Allarme low diff temperature del circuito secondario 2
    AL_LO_DIFF_TEMP_SC3,// Allarme low diff temperature del circuito secondario 3
    AL_TSC1,            // Allarme sonda del circuito secondario 1
    AL_TSC2,            // Allarme sonda del circuito secondario 2
    AL_TSC3,            // Allarme sonda del circuito secondario 3              //}
    AL_DESUP_HEAT1_TH,  // Allarme Desuperheat heater thermic 1                 //{ADD ACA STUPCW14.00 ECR #11150 : Aggiunti allarmi Desuperheat heater thermic
    AL_DESUP_HEAT2_TH,  // Allarme Desuperheat heater thermic 2                 //}
    AL_USER_DEF1,       // Allarme definito da utente                           //{ADD ACA STUPCW15.00 ECR #11657: Aggiunti allarmi definiti da utente
    AL_USER_DEF2,       // Allarme definito da utente                           //}
    AL_INV2_EVENT,      // allarme event 2 per screw con inverter                   // ADD MNS STUPCW18.00 oppio screw inverter
    AL_COND_PUMP_SH,    // allarme condenser pump share thermic                 // ADD ACA STUPW24.00 #16750 punto 1.4
    AL_WATCHDOG,        // allarme watchdog (solo segnalazione)                 // ADD ACA STUPCW25.00 #17492
    A_NUM
};


#endif /* SRC_APPLICATION_ALARM_H_ */
