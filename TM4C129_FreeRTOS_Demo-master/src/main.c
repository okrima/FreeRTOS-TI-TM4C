/* FreeRTOS 10 Tiva Demo
 *
 * main.c
 *
 * Andy Kobyljanec
 *
 * This is a simple demonstration project of FreeRTOS 8.2 on the Tiva Launchpad
 * EK-TM4C1294XL.  TivaWare driverlib sourcecode is included.
 */

#include <stdint.h>
#include <stdbool.h>
#include "sysctl.h"

//#include "driver.h"

//#include "main.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/debug.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "interrupt.h"

#include "driver_cSsi.h"
#include "driver_PWM.h"
#include "driver_TimerPWM.h"
#include "driver_TimerCounter.h"
#include "driver_ZeroCrossPhaseCut.h"
#include "driver_ADC.h"
#include "driver_Dig.h"
#include "S13_presence.h"

#include "TimerHdl.h"
#include "AnInOut.h"

#include "pindef__TM4C129X__.h"
#include "GestInOut.h"
#include "Controller.h"

const TickType_t millisecond = 1;
const TickType_t wait_10_ms = 10;

unsigned long GlobalTimer = 0;

unsigned char interrupt1msDisabled = 0;

unsigned char isS13Present = 0;


void init_S13()
{
    S13_Presence_Init();
}

void init_driver();

// 1 ms task to manage GlobalTimer and serial comunication
void global1msTask(void *pvParameters);

//Main application
void Application(void *pvParameters);

// Demo Task declarations
void demoLEDTask(void *pvParameters);
//void demoSerialTask(void *pvParameters);

// Main function
int main(void)
{
    // Initialize system clock to 120 MHz
    uint32_t output_clock_rate_hz;
    output_clock_rate_hz = SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               CLOCK);

    ASSERT(output_clock_rate_hz == CLOCK);

    //Initialize driver for TM4C129NCPDT
    init_driver();

    isS13Present = !S13_presence();

    IntMasterEnable();

    //Create 1ms task for timer and serial communication
    xTaskCreate(global1msTask, (const portCHAR *)"1MS_TASK",
                configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 2, NULL);

    //Main application
    xTaskCreate(Application, (const portCHAR *)"MAIN",
                configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 3, NULL);

    //START DRIVER TICKER
    xTaskCreate(HwADCTicker, (const portCHAR *)"ADC_TICK",
                configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);

    xTaskCreate(HwDigTicker, (const portCHAR *)"DIG_TICK",
                configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);

    xTaskCreate(HwTimerTick, (const portCHAR *)"TIMER_TICK",
                configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);

    xTaskCreate(HwDriverZCPCTicker, (const portCHAR *)"ZCPC_TICK",
                configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);


    vTaskStartScheduler();


    // Code should never reach this point
    return 0;
}

void changeLEDStatus()
{
    LedAppTgl();
}

unsigned long startTimerTemp = 0;

void blinkLedApp()
{
    TickType_t TimeSnap;

    interrupt1msDisabled = 1;
    TimeSnap = GlobalTimer;
    interrupt1msDisabled = 0;

    if((TimeSnap - startTimerTemp) > 1000)
    {
        startTimerTemp = TimeSnap;
        changeLEDStatus();
    }
}

//Main application
void Application(void *pvParameters)
{

    for(;;)
    {
        blinkLedApp();

        RefreshInput();

        RefreshOutput();

        Controller();

        vTaskDelay(2);
    }
}

int InternalTicker = 0;

// 1 ms task for GlobalTimer and serial communication
void global1msTask(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 1;

    TickType_t TickFrequency = pdMS_TO_TICKS(xFrequency);

    xLastWakeTime = xTaskGetTickCount ();

    for(;;)
    {
        vTaskDelayUntil( &xLastWakeTime, TickFrequency );

        if(!interrupt1msDisabled)
        {
            InternalTicker++;
            InternalTicker %= 1000;

            TimerHdl();                             //Timer

            switch (InternalTicker % 2) {
                case 0:
                    //TODO: COM
                    break;
                case 1:
                    //TODO: COM
                    break;
            }
        }
    }
}

/*
// Flash the LEDs on the launchpad
void demoLEDTask(void *pvParameters)
{
    for (;;)
    {
        // Turn on LED 1
        LEDWrite(0x0F, 0x01);
        vTaskDelay(1000);

        // Turn on LED 2
        LEDWrite(0x0F, 0x02);
        vTaskDelay(1000);

        // Turn on LED 3
        LEDWrite(0x0F, 0x04);
        vTaskDelay(1000);

        // Turn on LED 4
        LEDWrite(0x0F, 0x08);
        vTaskDelay(1000);
    }
}
*/

/*
// Write text over the Stellaris debug interface UART port
void demoSerialTask(void *pvParameters)
{
    // Set up the UART which is connected to the virtual COM port
    UARTStdioConfig(0, 57600, CLOCK);

    for (;;)
    {
        UARTprintf("Hello, world from FreeRTOS 10.2!\r\n");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
*/

void init_driver()
{
    //INIT DRIVER Ssi Expansion
    driverExpansionInit(4000000);

    //INIT DRIVER SD


    //INIT DRIVER ADC
    driverTM4C129X_ADCs_init();

    //INIT DRIVER DIGITAL INPUT & OUTPUT
    driver_Dig_init();

    //INIT DRIVER PWM
    init_driver_pwm();

    //INIT DRIVER TIMER COUNTER
    driverTimerCounterInit();

    //INIT DRIVER TIMER PWM
    driverTimerPwm();

    //INIT DRIVER ZERO CROSS PHASE CUT
    driver_zcpc();

    //INIT S13 PRESENCE
    init_S13();

    //INIT USER LED
    LedAppInit();
    LedAppOn();

}

/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName)
{
    while(1)
    {
    }
}
