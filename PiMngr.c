#include <stdint.h>
#include "PiMngr.h"
#include "PiMngr_Cfg.h"
#include "UART.h"
#include "UART_Cfg.h"
#include"app.h"

PiStates State;
Word Received_Word;
uint8_t RecognitionDone;

uint8_t RxBuff[20];
uint8_t RxBuff_Word[20];


void PiMngr_GetStatus(void)
{
    if(State == Initial)
    {
        UART_Tx_Init("Get Status", 10, UART_ChannelIdTx);
    }
    else
    {
        /* Do nothing */
    }
}

void PiMngr_StartRecognition(void)
{
    if(State == ReadytoRecognize)
    {
        //State = ReadytoRecognize;
        UART_Tx_Init("Start", 5, UART_ChannelIdTx);
    }
    else
    {
        /* Do nothing */
    }
}

void PiMngr_RequestData(void)
{
    if(State == ReadytoReceive)
    {
        UART_Tx_Init("Send", 4, UART_ChannelIdTx);
    }
    else
    {
        /* Do nothing */
    }
    if(State == ReceptionDone)
    {
        UART_Tx_Init("Done", 4, UART_ChannelIdTx);
        State = Doneeee;

    }
    else
    {
        /* Do nothing */
    }

}


void PiMngr_TxDone(void)
{
    if(State == Initial)
    {
        State = InitialInProg;
        UART_Rx_Init(RxBuff, 5, UART_ChannelIdRx);
        //Add another state called InitialInProg
        // This state is changed by the callback when reception is done

    }
    else if(State == ReadytoRecognize)
    {
        State = ReadytoRecognizeInProg;
        UART_Rx_Init(RxBuff, 7, UART_ChannelIdRx);

        //Add another state called ReadytoRecognizeInProg
        // This state is changed by the callback when reception is done

    }
    else if(State == ReadytoReceive)
    {
        State = ReceptionInProg;
        UART_Rx_Init(RxBuff_Word, 3, UART_ChannelIdRx);

        /***/
        // Add another state here called ReceptionInProg
        // This state is changed by the callback when reception is done


        /***/
    }
    else if(State == Doneeee)
    {


        PiMngr_Callback_Func();
        State = Initial;
        /*ID = RxBuff[0];
        DataLen = RxBuff[1];
        for(LoopIndex = 0; LoopIndex < DataLen; LoopIndex++)
        {
            Data[LoopIndex] = RxBuff[LoopIndex + 2];
        }*/
    }
    else
    {
        /* Do nothing */
    }

}

void PiMngr_RxDone(void)
{
    if(State == InitialInProg)
        // change the state in the if condition to InitialInProg
    {
        if((RxBuff[0] == 'R') && (RxBuff[1] == 'e') && (RxBuff[2] == 'a') && (RxBuff[3] == 'd') && (RxBuff[4] == 'y'))
        {
            State = ReadytoRecognize;
        }
        else if((RxBuff[0] == 'B') && (RxBuff[1] == 'u') && (RxBuff[2] == 's') && (RxBuff[3] == 'y') && (RxBuff[4] == 'y'))
        {
            State = Unpressed;
            // State = unpressed, not initial.
            // I'll make another state called unpressed
            // This state indicates that the button isn't pressed yet.
            // When it's pressed, the state will become initial and the pi manager will send get status
            // If the pi responds with ready, the flow will go on.
            // If the pi responds with busyy, the state will become unpressed again.
        }
    }
    else if(State == ReadytoRecognizeInProg)
        // change the state in the if condition to ReadytoRecognizeInProg
    {
        if((RxBuff[0] == 'S') && (RxBuff[1] == 't') && (RxBuff[2] == 'a') && (RxBuff[3] == 'r') && (RxBuff[4] == 't') && (RxBuff[5] == 'e') && (RxBuff[6] == 'd'))
        {
            State = ReadytoReceive;
        }
        else
        {
            //State = BusytoReceive;
        }
    }
    else if(State == ReceptionInProg)
        // change the state in the if condition to ReceptioninProg
    {
        // if received word is Navigation
        if((RxBuff_Word[0] == 'N') && (RxBuff_Word[1] == 'A') && (RxBuff_Word[2] == 'V'))
        {
            Received_Word = Pi_Navigation;
        }
        // if received word is Music
        else if((RxBuff_Word[0] == 'M') && (RxBuff_Word[1] == 'U') && (RxBuff_Word[2] == 'S'))
        {
            Received_Word = Pi_Music;
        }
        // if received word is Play
        else if((RxBuff_Word[0] == 'P') && (RxBuff_Word[1] == 'L') && (RxBuff_Word[2] == 'A'))
        {
            Received_Word = Pi_Play;
        }
        // if received word is Stop
        else if((RxBuff_Word[0] == 'S') && (RxBuff_Word[1] == 'T') && (RxBuff_Word[2] == 'O'))
        {
            Received_Word = Pi_Stop;
        }
        // if received word is Next
        else if((RxBuff_Word[0] == 'N') && (RxBuff_Word[1] == 'E') && (RxBuff_Word[2] == 'X'))
        {
            Received_Word = Pi_Next;
        }
        // if received word is Previous
        else if((RxBuff_Word[0] == 'P') && (RxBuff_Word[1] == 'R') && (RxBuff_Word[2] == 'E'))
        {
            Received_Word = Pi_Previous;
        }
        // if received word is Call
        else if((RxBuff_Word[0] == 'C') && (RxBuff_Word[1] == 'A') && (RxBuff_Word[2] == 'L'))
        {
            Received_Word = Pi_Call;
        }
        // if received word is Answer
        else if((RxBuff_Word[0] == 'A') && (RxBuff_Word[1] == 'N') && (RxBuff_Word[2] == 'S'))
        {
            Received_Word = Pi_Answer;
        }
        // if received word is Reject
        else if((RxBuff_Word[0] == 'R') && (RxBuff_Word[1] == 'E') && (RxBuff_Word[2] == 'J'))
        {
            Received_Word = Pi_Reject;
        }
        // if received word is Finish
        else if((RxBuff_Word[0] == 'F') && (RxBuff_Word[1] == 'I') && (RxBuff_Word[2] == 'N'))
        {
            Received_Word = Pi_Finish;
        }
        // if received word is Engine
        else if((RxBuff_Word[0] == 'E') && (RxBuff_Word[1] == 'N') && (RxBuff_Word[2] == 'G'))
        {
            Received_Word = Pi_Engine;
        }
        // if received word is Daniel
        else if((RxBuff_Word[0] == 'D') && (RxBuff_Word[1] == 'A') && (RxBuff_Word[2] == 'N'))
        {
            Received_Word = Pi_Daniel;
        }
        // if received word is Mark
        else if((RxBuff_Word[0] == 'M') && (RxBuff_Word[1] == 'A') && (RxBuff_Word[2] == 'R'))
        {
            Received_Word = Pi_Mark;
        }
        // if received word is Jessica
        else if((RxBuff_Word[0] == 'J') && (RxBuff_Word[1] == 'E') && (RxBuff_Word[2] == 'S'))
        {
            Received_Word = Pi_Jessica;
        }
        // if received word is not defined
        else if((RxBuff_Word[0] == 'E') && (RxBuff_Word[1] == 'R') && (RxBuff_Word[2] == 'R'))
        {
            Received_Word = Pi_Error;
        }
        else
        {
            //State = BusytoReceive;
        }
        State = ReceptionDone;

        //UART_Tx_Init("Done", 4, UART_ChannelIdTx);
    }
    else
    {
        /* Do nothing */
    }
}



void PiMngr_Callback_Func(void)
{
    RecognitionDone = 1;
    counter=0;
    if(Received_Word==Pi_Engine)
    {
    state=EngineHandling;
    }
    else if(Received_Word==Pi_Navigation)
    {
    state=GPSHandling;
    }
    else if(Received_Word==Pi_Call)
    {
    state=InCallHandling;
    }
    else if(Received_Word==Pi_Play)
      {
      state=MusicPlayHandling;
      }
    else if(Received_Word==Pi_Music)
      {
      state=MusicPlayHandling;
      }

    else if(Received_Word==Pi_Stop)
      {
      state=MusicStopHandling;
      }
    else if(Received_Word==Pi_Next)
      {
      state=NextHandling;
      }
    else if(Received_Word==Pi_Previous)
         {
         state=PrevHandling;
         }
    else if(Received_Word==Pi_Answer)
         {
         state=AnswerHandling;
         }
    else if(Received_Word==Pi_Error)
           {
           state=ErrorHandling;
           }
    else if(Received_Word==Pi_Reject)
           {
           state=RejectHandling;
           }


    else if(Received_Word==Pi_Finish)
              {
              state=EndHandling;
              }
    else if(Received_Word==Pi_Daniel)
              {
              state=Name1Hand;
              }
    else if(Received_Word==Pi_Mark)
              {
              state=Name2Hand;
              }
    else if(Received_Word==Pi_Jessica)
              {
              state=Name3Hand;
              }



    else
    {
        state = 255 ;
    }
}
