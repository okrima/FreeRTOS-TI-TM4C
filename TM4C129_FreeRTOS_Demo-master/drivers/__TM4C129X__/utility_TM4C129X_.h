/*
 * utility_TM4C129X_.h
 *
 *  Created on: 3 giu 2024
 *      Author: Mirko Albanese
 */


#ifndef DRIVERS___TM4C129X___UTILITY_TM4C129X__H_
#define DRIVERS___TM4C129X___UTILITY_TM4C129X__H_

#define disable() asm volatile(" cpsid i")  //Disable IRQ (Normal priority interrupt)
#define enable()  asm volatile(" cpsie i")  //Enable IRQ (Normal priority interrupt)

#define pENDOF(a)               (&(a)[FIELDCOUNT(a)])
#define STRUC_FOR(Var,Struct)   for(Var=&(Struct)[0];Var<pENDOF(Struct);Var++)
#define FIELDCOUNT(a)           ((uint32_t)(sizeof(a)/sizeof((a)[0])))

#define VALUE_ERROR                  ((int32_t)(0x80000000))
#define VALUE_ERROR_MAX              ((int32_t)(0x80000010))                    /* general value error maximum overflow  */

#define PINERR                       VALUE_ERROR

#ifndef PINERR_NOTEXISTS
#define PINERR_NOTEXISTS             ((int32_t)(PINERR|0x0000000))
#endif
#define PINERR_NOTREADY              ((int32_t)(PINERR|0x0000008))
#define PINERR_OPEN           VALUE_ERROR_MAX

#define HAL_HIGH   ((int32_t)10000000)                // 100%
#define HAL_LOW    ((int32_t)0)                       // 0%


// System clock rate, 120 MHz
#define CLOCK    120000000

#define PWM_SYSCLK_DIV_x    PWM_SYSCLK_DIV_32
#define PWM_SYSCLK_DIV                     32

int32_t driverGetValue(uint32_t Slot, uint32_t Channel);
void driverSetValue(uint32_t Slot, uint32_t Channel, int32_t value);

#endif /* DRIVERS___TM4C129X___UTILITY_TM4C129X__H_ */
