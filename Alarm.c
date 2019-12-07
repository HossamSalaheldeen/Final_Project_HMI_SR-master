#include <stdint.h>
#include "Alarm.h"
#include "led.h"


#define PI_LED  0U
#define CAN_LED 1U
#define BT_LED  2U
#define GPS_LED 3U
#define HMI_LED 4U

uint8_t Alarm_var;

Alarm_ChkType Alarm_Init (void)
{
    Alarm_ChkType RetVar;
    Alarm_var = 0x00;
    RetVar = Alarm_OK;
    return RetVar;

}

Alarm_ChkType Alarm_Managing_Function (void)
{
    Alarm_ChkType RetVar;
    if(Alarm_var < MAX_VALUE)
    {
        if((Alarm_var & (1U << PI_LED)) != 0)
        {
            LED_On(PI_LED);
        }
        else
        {
            LED_Off(PI_LED);

        }
        if((Alarm_var & (1U << CAN_LED)) != 0)
        {
            LED_On(CAN_LED);
        }
        else
        {
            LED_Off(CAN_LED);

        }
        if((Alarm_var & (1U << BT_LED)) != 0)
        {
            LED_On(BT_LED);
        }
        else
        {
            LED_Off(BT_LED);

        }
        if((Alarm_var & (1U << GPS_LED)) != 0)
        {
            LED_On(GPS_LED);
        }
        else
        {
            LED_Off(GPS_LED);

        }
        if((Alarm_var & (1U << HMI_LED)) != 0)
        {
            LED_On(HMI_LED);
        }
        else
        {
            LED_Off(HMI_LED);

        }

        RetVar = Alarm_OK;
    }
    else
    {
        RetVar = Alarm_NOK;
    }
    return RetVar;

}
