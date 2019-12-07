#include <stdint.h>
#include"Project_Includes.h"




uint8_t CAll_Request_State = 0;
extern uint32_t pp;
uint8_t StateX,x=0;
uint32_t counter=0,state=0;
uint32_t Time_Out_Error;
uint8_t BTH_Status_Check;

void app(void)
{


    if(state==ReadyToRecognize)
    {
       if(Connection_HFP_Status != BTH_Status_Check)
       {
           BTH_Status_Check = Connection_HFP_Status;
           BTH_Status_Display(0, BTH_Status_Check);
        }
        if(StateX == BTN_ON)
        {
            state=RecognitionRequested;
            StateX = BTN_OFF;
            HMI_RecognitionState(HMI_ID,RECORDING_START);
        }
    }
    if(state==RecognitionRequested)
    {
        Time_Out_Error++;
        counter++;
        if(counter==1)
        {
         /*Only Called One time to start the Recognition Request*/
        PiMngr_GetStatus();
        }
        /*Cyclic called until Recognition state is done*/
        PiMngr_StartRecognition();
        PiMngr_RequestData();
    }

    if(state==EngineHandling)
    {
        /*Indication that there is no time out error (word recepted and callback function changed the state)*/
        Time_Out_Error=0;
        /*End Recognition Screen on HMI and go back to home screen to begin the requested service */
        HMI_RecognitionState(HMI_ID,RECORDING_END);
        /* Transmission Engine Order via CAN Protocol */
        CAN0_Transmit_Data(1,ENGINE_STATUS_CHANGE);
        x^=1;
        /*Changing Engine status on Screen*/
        Engine_Status_Display(0,x);
        /*Service Requested and you are ready to recognize again */
        state =ReadyToRecognize;
        /*If there was an error then alarm LED will be off*/
        Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
    }
    if(state==GPSHandling)
    {
        Time_Out_Error=0;
        HMI_RecognitionState(HMI_ID,RECORDING_END);
        pp=0;
        /*GPS Service requested*/
        GPS_Request();
        state =ReadyToRecognize;
        Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
    }
    if(state==InCallHandling)
     {

         Time_Out_Error=0;
         HMI_RecognitionState(HMI_ID,RECORDING_END);
         CAll_Request_State=1;
         state =ReadyToRecognize;
         Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)

     }
    if(state == MusicPlayHandling)
    {

        //saved=Time_Out_Error;
        Time_Out_Error=0;
        HMI_RecognitionState(HMI_ID,RECORDING_END);
        state =ReadyToRecognize;
        Music_Status_Display(HMI_ID , Play);
        SendCmd(PlayMusic,"");
        Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
    }
    if(state == MusicStopHandling)
    {
        Time_Out_Error=0;

        HMI_RecognitionState(HMI_ID,RECORDING_END);
        state =ReadyToRecognize;
        Music_Status_Display(HMI_ID , Stop);
        SendCmd(PauseMusic,"");
        Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
    }
    if(state == NextHandling)
      {
          Time_Out_Error=0;
          HMI_RecognitionState(HMI_ID,RECORDING_END);
          state =ReadyToRecognize;
          Music_Status_Display(HMI_ID , Next);
          SendCmd(ForwardMusic,"");
          Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
      }
    if(state == PrevHandling)
      {
            Time_Out_Error=0;
            HMI_RecognitionState(HMI_ID,RECORDING_END);
            state =ReadyToRecognize;
            Music_Status_Display(HMI_ID , Previous);
            SendCmd(BackwardMusic,"");
            Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
      }
    if(state == AnswerHandling)
      {
            Time_Out_Error=0;
            HMI_RecognitionState(HMI_ID,RECORDING_END);
            SendCmd(BT_Answer,"");
            state =ReadyToRecognize;
            Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
      }
    if(state == RejectHandling)
      {
            Time_Out_Error=0;
            HMI_RecognitionState(HMI_ID,RECORDING_END);
            state =ReadyToRecognize;
            SendCmd(BT_Reject,"");
            Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
      }
    if(state == EndHandling)
      {
            Time_Out_Error=0;
            HMI_RecognitionState(HMI_ID,RECORDING_END);
            state =ReadyToRecognize;
            SendCmd(BT_End,"");
            Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
      }

    if((state == Name1Hand)&& (CAll_Request_State == 1))
        {
            Time_Out_Error=0;
            HMI_RecognitionState(HMI_ID,RECORDING_END);
            SendCmd(BT_Call,"01222497027");
            Incoming_Call_Display(HMI_ID,"01222497027"   );
            Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
            CAll_Request_State =0;
            state =ReadyToRecognize;
        }
    if((state == Name2Hand)&& (CAll_Request_State == 1))
        {
        Time_Out_Error=0;
        HMI_RecognitionState(HMI_ID,RECORDING_END);
        SendCmd(BT_Call,"01063254041");
        Incoming_Call_Display(0,"01063254041"  );
        Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
        CAll_Request_State =0;
        state =ReadyToRecognize;
        }
    if((state == Name3Hand)&& (CAll_Request_State == 1))
        {
        Time_Out_Error=0;
        HMI_RecognitionState(HMI_ID,RECORDING_END);
        SendCmd(BT_Call,"01065439954");
        Incoming_Call_Display(0,"01065439954" );
        Alarm_var=((~(1<<APP_PI_LED))&Alarm_var);//Clear bit (finished)
        CAll_Request_State =0;
        state =ReadyToRecognize;
        }
    if((state == Name3Hand)&& (CAll_Request_State == 0))
    {
            HMI_RecognitionState(HMI_ID,RECORDING_END);
            state =ReadyToRecognize;
            Time_Out_Error=0;
            State = Initial;
            counter=0;
    }

    if((state == Name2Hand)&& (CAll_Request_State == 0))
        {
            HMI_RecognitionState(HMI_ID,RECORDING_END);
            state =ReadyToRecognize;
            Time_Out_Error=0;
            State = Initial;
            counter=0;        }
    if((state == Name1Hand)&& (CAll_Request_State == 0))
        {
            HMI_RecognitionState(HMI_ID,RECORDING_END);
            state =ReadyToRecognize;
            Time_Out_Error=0;
            State = Initial;
            counter=0;
        }
    if(state == ErrorHandling)
   {
        HMI_RecognitionState(HMI_ID,RECORDING_END);
        state =ReadyToRecognize;
        Alarm_var |= (1U << APP_PI_LED);
        Time_Out_Error=0;
    }
    if(Time_Out_Error >= TIME_OUT_SCREEN)
    {
        HMI_RecognitionState(HMI_ID,RECORDING_END);
        state =ReadyToRecognize;
        Alarm_var |= (1U << APP_PI_LED);
        Time_Out_Error=0;
        State = Initial;
        counter=0;
    }
    }

