#include <stdint.h>
#include "GPS_CallBack.h"
#include "GPS.h"
#include "GPS_Cfg.h"
#include "HMIManager.h"
#include "HMIManager_Cfg.h"

extern uint8_t CoordinatesData[23];

//uint8_t *ptrtogps =CoordinatesData ;
uint32_t pp=0;
void LED_ON(void)
{
    pp++;
  //  ptrtogps[22]="\0";
    //CoordinatesData[22] = '\0' ;
    if(pp==1)
    {
    GPS_Status_Display( 0 ,CoordinatesData );
    }
}
void GPS_SelfTestFunc(void)
{
    SelfTest = GPS_SelfTestOK;
}
