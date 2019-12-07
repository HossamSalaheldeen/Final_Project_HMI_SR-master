#include"HMI.h"
#include"HMI_Cfg.h"
#include"HMIManager.h"
#include"HMIManager_Cfg.h"
#include "HMI_Manager_Callbacks.h"
#include "Alarm.h"
#include "led.h"


HMI_Callback_ChkType HMI_GetVersion_Callback_Flag;
HMI_Callback_ChkType HMI_SelectPicture_Callback_Flag;
HMI_Callback_ChkType HMI_GetPictureID_Callback_Flag;
HMI_Callback_ChkType HMI_SetDataVariable_Callback_Flag;
HMI_Callback_ChkType HMI_SetText_Callback_Flag;


void HMI_GetVersion_done(HMI_Callback_ChkType retVar)
{
    HMI_GetVersion_Callback_Flag = retVar;
    if((( HMI_Flag[Global_Loop_Index] & (1U << SELF_TEST))  != 0 )&& (HMI_State[Global_Loop_Index]  == HMI_BUSY ))
    {
        if (HMI_GetVersion_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            Alarm_var=((~(1<<4))&Alarm_var);//Clear bit (finished)
            HMI_Flag[Global_Loop_Index]=((~(1<<SELF_TEST))&HMI_Flag[Global_Loop_Index]);//Clear bit (finished)
            HMI_State[Global_Loop_Index] = HMI_READY;
        }
    }
}

void HMI_SelectPicture_done(HMI_Callback_ChkType retVar)
{
    volatile uint8_t counter=0;
    HMI_SelectPicture_Callback_Flag = retVar;
    if((( HMI_Flag[Global_Loop_Index] & (1U << RECOGNITION))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_BUSY ))
    {
        if (HMI_SelectPicture_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            HMI_Flag[Global_Loop_Index]=((~(1<<RECOGNITION))&HMI_Flag[Global_Loop_Index]);//Clear bit (finished)
            HMI_State[Global_Loop_Index] = HMI_READY ;
        }

    }
    if((( HMI_Flag[Global_Loop_Index] & (1U << ENGINE_STATUS))  != 0 )&& (HMI_State[Global_Loop_Index]  == HMI_BUSY ))
    {
        if (HMI_SelectPicture_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            HMI_State[Global_Loop_Index] = HMI_ENGINE1 ;
        }

    }
    if((( HMI_Flag[Global_Loop_Index] & (1U << ENGINE_STATUS))  != 0 )&& (HMI_State[Global_Loop_Index]  == HMI_ENGINE1_BUSY ))
    {
        if (HMI_SelectPicture_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            HMI_State[Global_Loop_Index] = HMI_ENGINE2 ;
        }

    }

    if((( HMI_Flag[Global_Loop_Index] & (1U << INCOMING_CALL))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_INCOMING_CALL1_BUSY ))// Incoming call request
    {
        if (HMI_SelectPicture_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            HMI_State[Global_Loop_Index] = HMI_INCOMING_CALL2 ;
        }
    }
    if((( HMI_Flag[Global_Loop_Index] & (1U << INCOMING_CALL))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_INCOMING_CALL2_BUSY ))// Incoming call request
    {
        if (HMI_SelectPicture_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            HMI_State[Global_Loop_Index]= HMI_ERASE_STATE;
        }

    }

    if((( HMI_Flag[Global_Loop_Index] & (1U << BTH_STATUS))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_BUSY ))// Incoming call request
    {
        if (HMI_SelectPicture_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            HMI_State[Global_Loop_Index] = HMI_BTH_STATUS1 ;
        }

    }
    if((( HMI_Flag[Global_Loop_Index] & (1U << BTH_STATUS))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_BTH_STATUS1_BUSY ))// Incoming call request
    {
        if (HMI_SelectPicture_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            HMI_Flag[Global_Loop_Index]=((~(1<<BTH_STATUS))&HMI_Flag[Global_Loop_Index]);//Clear bit (finished)
            HMI_State[Global_Loop_Index] = HMI_READY ;
        }
    }
	
    if((( HMI_Flag[Global_Loop_Index] & (1U << GPS_STATUS))  != 0 )&& (HMI_State[Global_Loop_Index]  == HMI_BUSY ))
    {
        if (HMI_SelectPicture_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            HMI_State[Global_Loop_Index] = HMI_GPS_STATUS1 ;
        }
    }
    if((( HMI_Flag[Global_Loop_Index] & (1U << GPS_STATUS))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_GPS_STATUS2_BUSY ))
    {
        if (HMI_SelectPicture_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            HMI_Flag[Global_Loop_Index]=((~(1<<GPS_STATUS))&HMI_Flag[Global_Loop_Index]);//Clear bit (finished)
            HMI_State[Global_Loop_Index] = HMI_READY ;
        }
    }


}

void HMI_GetPictureID_done(HMI_Callback_ChkType retVar)
{
    HMI_GetPictureID_Callback_Flag = retVar;
}


void HMI_SetDataVariable_done(HMI_Callback_ChkType retVar)
{
    HMI_SetDataVariable_Callback_Flag = retVar;
}


void HMI_SetText_done(HMI_Callback_ChkType retVar)
{
    HMI_SetText_Callback_Flag = retVar;
    if((( HMI_Flag[Global_Loop_Index] & (1U << ENGINE_STATUS))  != 0 )&& (HMI_State[Global_Loop_Index]  == HMI_ENGINE2_BUSY ))
    {
        if (HMI_SetText_Callback_Flag == HMI_Callback_NOK )
        {
            HMI_State[Global_Loop_Index] = HMI_ERROR;
        }
        else
        {
            HMI_Flag[Global_Loop_Index]=((~(1<<ENGINE_STATUS)) & HMI_Flag[Global_Loop_Index]);//Clear bit (finished)
            HMI_State[Global_Loop_Index] = HMI_READY ;
        }
    }
        if((( HMI_Flag[Global_Loop_Index] & (1U << MUSIC_STATUS))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_BUSY ))// Music status request
        {
            if (HMI_SetText_Callback_Flag == HMI_Callback_NOK )
            {
                HMI_State[Global_Loop_Index] = HMI_ERROR;
            }
            else
            {
                if((Music_Status_Flag == Play) || (Music_Status_Flag == Pause) || (Music_Status_Flag == Stop))
				{
					HMI_Flag[Global_Loop_Index]=((~(1<<MUSIC_STATUS)) & HMI_Flag[Global_Loop_Index]);//Clear bit (finished)
					HMI_State[Global_Loop_Index] = HMI_READY ;
				}
                /*If Previous or Next, You have to wait some time and go to Playing/Paused/Stop*/
                else if((Music_Status_Flag == Previous)|| (Music_Status_Flag == Next))
				{
					HMI_State[Global_Loop_Index] = HMI_MUSIC_STATUS1 ;
				}

            }
        }

        if((( HMI_Flag[Global_Loop_Index] & (1U << MUSIC_STATUS))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_MUSIC_STATUS1_BUSY ))// Music status request
        {
            if (HMI_SetText_Callback_Flag == HMI_Callback_NOK )
            {
                HMI_State[Global_Loop_Index] = HMI_ERROR;
            }
            else
            {
                HMI_Flag[Global_Loop_Index]=((~(1<<MUSIC_STATUS)) & HMI_Flag[Global_Loop_Index]);//Clear bit (finished)
                HMI_State[Global_Loop_Index] = HMI_READY ;
            }

        }
        if((( HMI_Flag[Global_Loop_Index] & (1U << MUSIC_NAME))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_BUSY ))
        {
            if (HMI_SetText_Callback_Flag == HMI_Callback_NOK )
            {
                HMI_State[Global_Loop_Index] = HMI_ERROR;
            }
            else
            {
                HMI_Flag[Global_Loop_Index]=((~(1<<MUSIC_NAME)) & HMI_Flag[Global_Loop_Index]);//Clear bit (finished)
                HMI_State[Global_Loop_Index] = HMI_READY ;
            }
        }

        if((( HMI_Flag[Global_Loop_Index] & (1U << INCOMING_CALL))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_BUSY ))// Incoming call request
        {
            if (HMI_SetText_Callback_Flag == HMI_Callback_NOK )
            {
                HMI_State[Global_Loop_Index] = HMI_ERROR;
            }
            else
            {
                    HMI_State[Global_Loop_Index] = HMI_INCOMING_CALL1 ;
            }

        }
        if((( HMI_Flag[Global_Loop_Index] & (1U << INCOMING_CALL))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_ERASE_STATE_BUSY ))// Incoming call request
        {
            if (HMI_SelectPicture_Callback_Flag == HMI_Callback_NOK )
            {
                HMI_State[Global_Loop_Index] = HMI_ERROR;
            }
            else
            {
                HMI_Flag[Global_Loop_Index]=((~(1<<INCOMING_CALL))&HMI_Flag[Global_Loop_Index]);//Clear bit (finished)
                HMI_State[Global_Loop_Index] = HMI_READY ;
            }

        }
        if((( HMI_Flag[Global_Loop_Index] & (1U << GPS_STATUS))  != 0 )&& (HMI_State[Global_Loop_Index] == HMI_GPS_STATUS1_BUSY ))
        {
            if (HMI_SetText_Callback_Flag == HMI_Callback_NOK )
            {
                HMI_State[Global_Loop_Index] = HMI_ERROR;
            }
            else
            {
                HMI_State[Global_Loop_Index] = HMI_GPS_STATUS2 ;
            }
        }
}

