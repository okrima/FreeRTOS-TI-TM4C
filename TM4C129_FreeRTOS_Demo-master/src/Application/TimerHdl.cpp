#include "TimerHdl.h"

unsigned char UnitTime = 1;


void TimerHdl();

void TimerHdl()
{
    GlobalTimer += UnitTime;
}
