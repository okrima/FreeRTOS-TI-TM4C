#include "S13_presence.h"

// MNS S13 presence
void S13_Presence_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    GPIOPinTypeGPIOInput(GPIO_PORTP_BASE,GPIO_PIN_2);
}
unsigned int S13_presence(void)
{
	if (GpioPort(P)&0x04)
		return 1;
	else
		return 0;

}


