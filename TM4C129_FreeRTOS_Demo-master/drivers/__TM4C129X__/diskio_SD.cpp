#include "stdint.h"

#include "diskio_SD.h"
#include "utility_TM4C129X_.h"

#include "pindef__TM4C129X__.h"

unsigned int diskio_SD_socket_is_empty(void)
{
    static uint8_t init = 0;
    if(!init)
    {
        GpioPortInitBitIn(N,2);
        init=1;
    }
    return GpioPortBitGet(N,2);
}

