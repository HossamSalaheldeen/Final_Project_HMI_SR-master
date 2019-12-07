#include "HMI.h"
#include "HMIManager.h"
#include "HMIManager_Cfg.h"
#include "HMI_Manager_Callbacks.h"
#include "Alarm.h"
#include "led.h"

uint8_t Engine_Status_Flag=1 ,Music_Status_Flag=0,BTH_Status_Flag=1,RecognitionState_Flag=0,Call_Decision;
uint8_t Imported_Number[15];
uint8_t * Ptr_Imported_Number = Imported_Number;
uint8_t GPS_Data[23];
uint8_t * Ptr_GPS_Data = GPS_Data;
uint8_t MusicName[18];
uint8_t * Ptr_MusicName = MusicName;


uint32_t BTH_Delay_Counter=0,GPS_Delay_Counter=0 ,IncomingCall_Delay_Counter=0 ,Music_Delay_Counter=0,Engine_Delay_Counter=0;
uint8_t Busy_Delay=0;

uint8_t Global_Loop_Index;
/*States for every HMI defined as the states going from READY until reaching READY again or ERROR */
STATES HMI_State[HMIManager_CHANNELS_NUMBER] = {HMI_UNINIT};
/* A register like setting for flags with a default state of uninitialized for all HMI's*/
uint16_t HMI_Flag[HMIManager_CHANNELS_NUMBER]={0};

HMIManager_ChkType RetVar ;

HMIManager_ChkType HMI_Managing_Function(void)
{

    uint8_t Loop_Index;
    const HMIManager_ConfigType* CfgPtr;
    for(Loop_Index=0 ; Loop_Index<HMIManager_CHANNELS_NUMBER ; Loop_Index++)
    {
			Global_Loop_Index = Loop_Index;
			CfgPtr = &HMIManager_ConfigParam[Loop_Index];
			/* made all of the statements if and not else if so that we can increment it's counter every time the manage function is called*/
			/*Intializing Service*/
            if((( HMI_Flag[Loop_Index] & (1U << INIT))  != 0 )&& (HMI_State[Loop_Index]  == HMI_UNINIT )) 
            {
                HMI_State[Loop_Index] = HMI_BUSY;
				/*Clear Service bit (finished)*/
                HMI_Flag[Loop_Index]=((~(1<<INIT))&HMI_Flag[Loop_Index]);
				HMI_State[Loop_Index]= HMI_INIT_DONE;
            }
			/*Self test request*/
            if((( HMI_Flag[Loop_Index] & (1U << SELF_TEST))  != 0 )&& (HMI_State[Loop_Index]  == HMI_SELF_TEST )) 
            {
                /*Performing the check on HMI ID used in both HMI and HMIManager is the same*/
                HMI_State[Loop_Index] = HMI_BUSY;
                HMI_GetVersion(Loop_Index);
            }
			/*Engine Status Display Service
			*=The main concept of Service is to serve the demanded Request when the state is READY
			*=If your servise contains more than one step from HMI_Driver, It will be divides into states 
			*and be busy in this state to be served and at the end you go READY or ERROR states */
            if((( HMI_Flag[Loop_Index] & (1U << RECOGNITION))  != 0 )&& (HMI_State[Loop_Index] == HMI_READY ))
            {
                HMI_State[Loop_Index] = HMI_BUSY;
                if(RecognitionState_Flag == 1)
                {
                    HMI_SelectPicture(Loop_Index,CfgPtr->HMI_RecognitionStarted_ID);
                }
                else
                {
                    HMI_SelectPicture(Loop_Index,CfgPtr->HMI_HomeScreen_ID);
                }
            }
           if((( HMI_Flag[Loop_Index] & (1U << ENGINE_STATUS))  != 0 )&& (HMI_State[Loop_Index]  == HMI_READY ))    
            {
                    HMI_State[Loop_Index]=HMI_BUSY;
					/*Checks for The Engine State*/
                    if((Engine_Status_Flag == Engine_ON))
                    {
                        HMI_SelectPicture(Loop_Index,CfgPtr->HMI_EngineON_ID);
                    }
                    else
                    {
                        HMI_SelectPicture(Loop_Index,CfgPtr->HMI_EngineOFF_ID);
                    }
            }
           if((( HMI_Flag[Loop_Index] & (1U << ENGINE_STATUS))  != 0 )&& (HMI_State[Loop_Index]  == HMI_ENGINE1 ))    
            {
				/*Engine_Delay_Counter is defined as the number of the cycles that the Managing Function will Passby the
				*return to Home Screen as we need to introduce a delay for our notification 
				*--ENGINE_DELAY = Time Delay Needed / Managing Function Cyclic time  */
				Engine_Delay_Counter++;
				if(Engine_Delay_Counter >= ENGINE_DELAY)
				{
					HMI_State[Loop_Index]=HMI_ENGINE1_BUSY;
					Engine_Delay_Counter=0;
					HMI_SelectPicture(Loop_Index,CfgPtr->HMI_HomeScreen_ID);
				}
            }
			if((( HMI_Flag[Loop_Index] & (1U << ENGINE_STATUS))  != 0 )&& (HMI_State[Loop_Index]  == HMI_ENGINE2 ))    
			{
				HMI_State[Loop_Index]=HMI_ENGINE2_BUSY;
				if((Engine_Status_Flag == 1))
				{
					HMI_SetText(Loop_Index,CfgPtr->HMI_EngineStatusSetText_ID, "ON ");
				}
				else
				{	
					HMI_SetText(Loop_Index,CfgPtr->HMI_EngineStatusSetText_ID, "OFF");
				}
			}
			/*Music status Display Service*/
			if((( HMI_Flag[Loop_Index] & (1U << MUSIC_STATUS))  != 0 )&& (HMI_State[Loop_Index] == HMI_READY ))
            {
                    HMI_State[Loop_Index] = HMI_BUSY;
                    if((Music_Status_Flag == Play))
                        {
                            HMI_SetText(Loop_Index,CfgPtr->HMI_MusicStatus_ID, "Playing     ");
                        }
                        else if((Music_Status_Flag == Pause))
                        {
                            HMI_SetText(Loop_Index,CfgPtr->HMI_MusicStatus_ID, "Pause     ");
                        }
                        else if((Music_Status_Flag == Stop))
                        {
                            HMI_SetText(Loop_Index,CfgPtr->HMI_MusicStatus_ID, "Stop     ");
                        }
                        else if((Music_Status_Flag == Previous))
                        {
                            HMI_SetText(Loop_Index,CfgPtr->HMI_MusicStatus_ID, "Backward    ");
                        }
                        else if((Music_Status_Flag == Next))
                        {
                            HMI_SetText(Loop_Index,CfgPtr->HMI_MusicStatus_ID, "Forward        ");
                        }
            }
			if((( HMI_Flag[Loop_Index] & (1U << MUSIC_STATUS))  != 0 )&& (HMI_State[Loop_Index] == HMI_MUSIC_STATUS1 ))
                {
                    Music_Delay_Counter++;
                    if(Music_Delay_Counter >= MUSIC_DELAY)
                    {
                        HMI_State[Loop_Index]=HMI_MUSIC_STATUS1_BUSY;
                        Music_Delay_Counter=0;
                        HMI_SetText(Loop_Index,CfgPtr->HMI_MusicStatus_ID, "Playing          ");
                    }
                }
            if((( HMI_Flag[Loop_Index] & (1U << MUSIC_NAME))  != 0 )&& (HMI_State[Loop_Index] == HMI_READY ))
            {
                    HMI_State[Loop_Index] = HMI_BUSY;
                    HMI_SetText(Loop_Index,CfgPtr->HMI_MusicName_ID, Ptr_MusicName);
            }
			/*Incoming Call Display Service*/
            if((( HMI_Flag[Loop_Index] & (1U << INCOMING_CALL))  != 0 )&& (HMI_State[Loop_Index] == HMI_READY ))
            {
                HMI_State [Loop_Index]= HMI_BUSY;
                HMI_SetText(Loop_Index,CfgPtr->HMI_ContactNumber_ID,Ptr_Imported_Number );
            }
            if((( HMI_Flag[Loop_Index] & (1U << INCOMING_CALL))  != 0 )&& (HMI_State[Loop_Index] == HMI_INCOMING_CALL1 ))
            {
                HMI_State[Loop_Index]=HMI_INCOMING_CALL1_BUSY;
                HMI_SelectPicture(Loop_Index,CfgPtr->HMI_IncomingCall_ID);
            }
            if((( HMI_Flag[Loop_Index] & (1U << INCOMING_CALL))  != 0 )&& (HMI_State[Loop_Index] == HMI_INCOMING_CALL2 ))
            {
                IncomingCall_Delay_Counter++;

                if(Call_Decision == Answer)
                {
                    IncomingCall_Delay_Counter=(INCOMINGCALL_DELAY-500);
                    Call_Decision = NoDecision ;
                    HMI_SetText(Loop_Index,CfgPtr->HMI_CallText_ID,"CALL ANSWERED        " );
                }
                else if(Call_Decision == Reject)
                {
                    IncomingCall_Delay_Counter=(INCOMINGCALL_DELAY-500);
                    Call_Decision = NoDecision ;
                    HMI_SetText(Loop_Index,CfgPtr->HMI_CallText_ID,"CALL REJECTED        " );
                }

                if(IncomingCall_Delay_Counter >= INCOMINGCALL_DELAY)
                {
                    HMI_State[Loop_Index]=HMI_INCOMING_CALL2_BUSY;
                    IncomingCall_Delay_Counter=0;
                    HMI_SelectPicture(Loop_Index,CfgPtr->HMI_HomeScreen_ID);
                }
            }
            if((( HMI_Flag[Loop_Index] & (1U << INCOMING_CALL))  != 0 )&& (HMI_State[Loop_Index] == HMI_ERASE_STATE ))
            {
                HMI_State[Loop_Index] = HMI_ERASE_STATE_BUSY ;
            HMI_SetText(Loop_Index,CfgPtr->HMI_CallText_ID,"                    " );
            }
			/*Bluetooth Status Display Service*/
            if((( HMI_Flag[Loop_Index] & (1U << BTH_STATUS))  != 0 )&& (HMI_State[Loop_Index] == HMI_READY ))
            {
                HMI_State[Loop_Index]=HMI_BUSY;
                if(BTH_Status_Flag == 0)
                {
                    HMI_SelectPicture(Loop_Index,CfgPtr->HMI_BTHStatusConnected_ID);
                }
                else
                {
                    HMI_SelectPicture(Loop_Index,CfgPtr->HMI_BTHStatusDisconnected_ID);
                }
            }
            if((( HMI_Flag[Loop_Index] & (1U << BTH_STATUS))  != 0 )&& (HMI_State[Loop_Index] == HMI_BTH_STATUS1 ))
            {
                    BTH_Delay_Counter++;
                     if(BTH_Delay_Counter >= BTH_DELAY)
                     {
                         HMI_State[Loop_Index]=HMI_BTH_STATUS1_BUSY;
                         BTH_Delay_Counter=0;
                         HMI_SelectPicture(Loop_Index,CfgPtr->HMI_HomeScreen_ID);
                     }
            }
			/*GPS Status Display Service*/
            if((( HMI_Flag[Loop_Index] & (1U << GPS_STATUS))  != 0 )&& (HMI_State[Loop_Index]  == HMI_READY ))  
             {
                     HMI_State[Loop_Index]=HMI_BUSY;
                     HMI_SelectPicture(Loop_Index,CfgPtr->HMI_GPSDisplay_ID);
             }
            if((( HMI_Flag[Loop_Index] & (1U << GPS_STATUS))  != 0 )&& (HMI_State[Loop_Index] == HMI_GPS_STATUS1 ))
            {
                         HMI_State[Loop_Index]=HMI_GPS_STATUS1_BUSY;
                         HMI_SetText(0,CfgPtr->HMI_GPSData_ID , Ptr_GPS_Data);
            }
            if((( HMI_Flag[Loop_Index] & (1U << GPS_STATUS))  != 0 )&& (HMI_State[Loop_Index] == HMI_GPS_STATUS2 ))
            {
				GPS_Delay_Counter++;
				if(GPS_Delay_Counter >= GPS_DELAY)
				{
					HMI_State[Loop_Index]=HMI_GPS_STATUS2_BUSY;
					GPS_Delay_Counter=0;
					HMI_SelectPicture(Loop_Index,CfgPtr->HMI_HomeScreen_ID);
				}
            }



            RetVar= HMIManager_OK;
			/*ERROR state takes the state to SELF TEST and perform the test again for proceeding the Screen to begin Service State [READY]*/
            if(HMI_State[Loop_Index]==HMI_ERROR)
            {
                Alarm_var |= (1U << 4);
                RetVar = HMIManager_NOK;
                HMI_Flag[Loop_Index] |= (1U << SELF_TEST);
                HMI_State[Loop_Index]  = HMI_SELF_TEST;
            }
			/*Busy For a long time means that there is Error in connections and needs to go to ERROR state 
			*and then perform a SELF TEST  again*/
            if(HMI_State[Loop_Index]==HMI_BUSY)
            {
                Busy_Delay ++;
                if(Busy_Delay == 100)
                {
                    Busy_Delay=0;
                    HMI_State[Loop_Index]=HMI_ERROR;
                    RetVar = HMIManager_NOK;
                }
            }
    }
return RetVar;
}





/*Service functions raises a flag which is not pending waiting for the managing function to serve Periodicaly  */
HMIManager_ChkType Manager_Init(void)
{
     HMIManager_ChkType RetVar;
     uint8_t Loop_Index;
     if((HMI_State[Global_Loop_Index] == HMI_UNINIT ) && ( ( HMI_Flag[Global_Loop_Index] &(1U <<INIT) ) == 0 ) )/*flag not high (not pending)*/
     {
     for(Loop_Index=0;Loop_Index<HMIManager_CHANNELS_NUMBER;Loop_Index++)
     {
         HMI_Flag[Loop_Index]= 1U << INIT;
     }
     RetVar= HMIManager_OK;
     return RetVar;
     }
     else
     {
         RetVar= HMIManager_NOK;
         return RetVar;
     }
}


HMIManager_ChkType Manager_Self_Test(uint8_t HMI_groupID)
{
    HMIManager_ChkType RetVar;

    if(HMI_groupID < HMIManager_CHANNELS_NUMBER)
    {
       if ( ( HMI_Flag[HMI_groupID] &(1U <<SELF_TEST) ) == 0 )  // the driver is in the init done state and the same request isn't pending
       {
        HMI_Flag[HMI_groupID] |= (1U << SELF_TEST);
        HMI_State[HMI_groupID]  = HMI_SELF_TEST;
        RetVar=HMIManager_OK;
       }
       else
       {
           RetVar=HMIManager_NOK;
       }
    }
    else
    {
        RetVar = HMIManager_NOK;
    }
    return RetVar;
}


HMIManager_ChkType Engine_Status_Display(uint8_t HMI_groupID , uint8_t Engine_Status) // 0 for off 1 for on

{
   HMIManager_ChkType RetVar;
   if(HMI_groupID < HMIManager_CHANNELS_NUMBER )
   {
     //  if ( ( HMI_Flag[HMI_groupID] &(1U <<ENGINE_STATUS) ) == 0 )  // the same request isn't pending
       //{
           HMI_Flag[HMI_groupID] |= (1U << ENGINE_STATUS);
           Engine_Status_Flag = Engine_Status;
           RetVar=HMIManager_OK;
       //}
      // else
       //{
         //  RetVar=HMIManager_NOK;
       //}
   }
   else
   {
     RetVar= HMIManager_NOK;
   }
return RetVar;
}

HMIManager_ChkType Music_Status_Display(uint8_t HMI_groupID,uint8_t Music_Status)
{
    HMIManager_ChkType RetVar;
       if(HMI_groupID < HMIManager_CHANNELS_NUMBER )
       {
           if(  ( HMI_Flag[HMI_groupID] &(1U << MUSIC_STATUS) ) == 0 )  // the same request isn't pending
           {
               HMI_Flag[HMI_groupID] |= (1U << MUSIC_STATUS);
               Music_Status_Flag= Music_Status;
               RetVar=HMIManager_OK;
           }
           else
           {
               RetVar=HMIManager_NOK;
           }
       }
       else
       {
         RetVar= HMIManager_NOK;
       }

    return RetVar;

}
HMIManager_ChkType HMI_MusicName_Display(uint8_t HMI_groupID  , uint8_t Local_MusicName[] )
{
    uint8_t Local_LoopIndex;
    HMIManager_ChkType RetVar;
       if(HMI_groupID < HMIManager_CHANNELS_NUMBER )
       {
           if(  ( HMI_Flag[HMI_groupID] &(1U << MUSIC_NAME) ) == 0 )  // the same request isn't pending
           {
               HMI_Flag[HMI_groupID] |= (1U << MUSIC_NAME);
               for(Local_LoopIndex=0;Local_LoopIndex<18;Local_LoopIndex++)
               {
               MusicName[Local_LoopIndex]= Local_MusicName[Local_LoopIndex];
               }
                  MusicName[17]='\0';
               RetVar=HMIManager_OK;
           }
           else
           {
               RetVar=HMIManager_NOK;
           }
       }
       else
       {
         RetVar= HMIManager_NOK;
       }

    return RetVar;

}







HMIManager_ChkType Incoming_Call_Display(uint8_t HMI_groupID, uint8_t Number[]  )
{
    HMIManager_ChkType RetVar;
    uint8_t Local_LoopIndex;
       if(HMI_groupID < HMIManager_CHANNELS_NUMBER )
       {
           if(  ( HMI_Flag[HMI_groupID] &(1U << INCOMING_CALL) ) == 0 ) // the same request isn't pending
           {
               for(Local_LoopIndex=0;Local_LoopIndex<=11;Local_LoopIndex++)
               {
               Imported_Number[Local_LoopIndex]= Number[Local_LoopIndex];
               }
               HMI_Flag[HMI_groupID] |= (1U << INCOMING_CALL);
               RetVar=HMIManager_OK;
           }
           else
           {
               RetVar=HMIManager_NOK;
           }
       }
       else
       {
         RetVar= HMIManager_NOK;
       }

    return RetVar;


}

HMIManager_ChkType BTH_Status_Display(uint8_t HMI_groupID , uint8_t BTH_Status)

{
    HMIManager_ChkType RetVar;
    if(HMI_groupID < HMIManager_CHANNELS_NUMBER )
    {
        if(  ( HMI_Flag[HMI_groupID] &(1U << BTH_STATUS) ) == 0 ) //  the same request isn't pending
        {
            BTH_Status_Flag = BTH_Status ;
            HMI_Flag[HMI_groupID] |= (1U << BTH_STATUS);
            RetVar=HMIManager_OK;
        }
        else
        {
            RetVar=HMIManager_NOK;
        }
    }
    else
    {
        RetVar= HMIManager_NOK;
    }
    return RetVar;
}

HMIManager_ChkType GPS_Status_Display( uint8_t HMI_groupID  , uint8_t Local_GPSData[])
{
    HMIManager_ChkType RetVar;
    uint8_t Local_LoopIndex;
    if(HMI_groupID < HMIManager_CHANNELS_NUMBER  )
    {
        if( ( HMI_Flag[HMI_groupID] &(1U << GPS_STATUS) ) == 0 )  //  the same request isn't pending
        {
            for(Local_LoopIndex=0;Local_LoopIndex<=GPS_DATA_LENGTH;Local_LoopIndex++)
            {
            GPS_Data[Local_LoopIndex]= Local_GPSData[Local_LoopIndex];
            }
            HMI_Flag[HMI_groupID] |= (1U << GPS_STATUS);
            RetVar=HMIManager_OK;
        }
        else
        {
              RetVar=HMIManager_NOK;
        }
    }
    else
    {
        RetVar= HMIManager_NOK;
    }
    return RetVar;
}

HMIManager_ChkType HMI_RecognitionState(uint8_t HMI_groupID  , uint8_t Rec_State ) //1 start 0 end
{

    HMIManager_ChkType RetVar;
    if(HMI_groupID < HMIManager_CHANNELS_NUMBER )
    {

            RecognitionState_Flag = Rec_State ;
            HMI_Flag[HMI_groupID] |= (1U << RECOGNITION);
            RetVar=HMIManager_OK;

    }
    else
    {
        RetVar= HMIManager_NOK;
    }
    return RetVar;

}

HMIManager_ChkType HMI_CallDecision (uint8_t HMI_groupID , uint8_t Decision)
{

    HMIManager_ChkType RetVar;
    if(HMI_groupID < HMIManager_CHANNELS_NUMBER )
    {

            Call_Decision = Decision;
            RetVar=HMIManager_OK;
    }
    else
    {
        RetVar= HMIManager_NOK;
    }
    return RetVar;

}

