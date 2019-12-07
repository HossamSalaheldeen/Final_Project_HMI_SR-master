#include <stdint.h>
#include "UART.h"
#include "BT.h"
#include "BT_Cfg.h"

uint8_t RcvBuff[RBuff];
uint16_t Write;
uint16_t WriteOld;
uint16_t Read;
uint32_t xx=0;
static uint16_t LoopIndexFix(uint8_t LoopIndex);
static uint8_t Difference(void);
static uint8_t Compaire(uint8_t CheckTxt[], uint8_t CompairLen);
uint64_t ara=0;
void BT_Init(void)
{
    BT_State = Startup;
    Call_Status = CALL_NUTRAL;
    Connection_HFP_Status = CON_HFP_OFF;
    Connection_SPP_Status = CON_SPP_OFF;
    Response = CMD_NUTRAL;
    Bt_Status = BT_NPOWERUP;
    CallIn = NO_CALL;
    Write = 0;
    Read = 0;
    UART_Rx_Init(&RcvBuff[Write], 1, UART_ChannelID_Rx);
}

void BT_Cyclic(void)
{
    if(Call_Status == CALL_START)
    {
        ara++;
    }
    uint8_t Flag;
    uint8_t Diff;
    uint8_t LoopIndexCyclic;
    uint16_t NewIndex;
    UART_Rx(UART_ChannelID_Rx);

    Diff = Difference();

    if((Diff > 0) && (RcvBuff[WriteOld] == '\r'))
    {
        if(BT_State == Startup)
        {
            if(Difference() == 7)
            {
                Flag = Compaire("Ready", 5);
                if(Flag == 1)
                {
                    BT_State = WaitConnection;
                    Read = Write;
                    Bt_Status = BT_POWERUP;
                }
                else
                {
                    /*    Bt_Status = BT_NPOWERUP;
                    BT_State = Restart;
                    BT_Init();
                    SendCmd(Reset, "");
                    */
                }
            }
            else
            {
                Read = Write;
            }
        }
        else if(BT_State == WaitConnection)
        {
            if(Diff == 13)
            {
                Flag = Compaire("OPEN_OK HFP", 11);
                if(Flag == 1)
                {
                    BT_State = WaitResponse;
                    Connection_HFP_Status = CON_HFP_ON;
                    Read = Write;
                }
                else if(Flag == 0)
                {
                    Flag = Compaire("OPEN_OK SPP", 11);
                    if(Flag == 1)
                    {
                        BT_State = WaitResponse;
                        Connection_SPP_Status = CON_SPP_ON;
                        Read = Write;
                    }
                }
                else
                {
                    Read = Write;;
                }
            }
            else if(Diff == 15)
            {
                Flag = Compaire("OPEN_ERROR AG", 13);
                if(Flag == 1)
                {
                    BT_State = NeedConfiguration;
                    Connection_HFP_Status = CON_HFP_FAIL;
                    Read = Write;
                    BT_Init();
                    SendCmd(Configuration, "");
                }
            }
            else
            {
                Read = Write;
            }

        }
        else if(BT_State == NeedConfiguration)
        {
            if(Diff == 4)
            {
                Flag = Compaire("OK", 2);
                if(Flag == 1)  //check comp.
                {
                    Response = CMD_OK;
                    Read = Write;
                }
                else
                {
                    Read = Write;
                }
            }
            else if(Diff == 7)
            {
                Flag = Compaire("ERROR", 5);
                if(Flag == 1)
                {
                    Response = CMD_ERROR;
                    Read = Write;
                }
                else
                {
                    Read = Write;
                }
            }
            else
            {
                Read = Write;
            }

        }
        else if(BT_State == WaitResponse)
        {
            if(Diff == 4)
            {
                Flag = Compaire("OK", 2);
                if(Flag == 1)
                {
                    Response = CMD_OK;
                    Read = Write;
                }
                else
                {
                    Read = Write;
                }
            }
            else if(Diff == 7)
            {
                Flag = Compaire("ERROR", 5);
                if(Flag == 1)
                {
                    Response = CMD_ERROR;
                    Read = Write;
                }
                else
                {
                    Read = Write;
                }
            }
            else if(Diff == 10)
            {
                Flag = Compaire("CALL_END", 8);
                if(Flag == 1)
                {
                    Call_Status = CALL_END;
                    Read = Write;
                }
                else
                {
                    Read = Write;
                }
            }
            else if(Diff == 13)
            {
                Flag = Compaire("CALL_ACTIVE", 11);
                if(Flag == 1)
                {
                    Call_Status = CALL_START;
                    Read = Write;
                }
                else if(Flag == 0)
                {
                    Flag = Compaire("OPEN_OK HFP", 11);
                    if(Flag == 1)
                    {
                        Connection_HFP_Status = CON_HFP_ON;
                        Read = Write;
                    }
                    else if(Flag == 0)
                    {
                        Flag = Compaire("OPEN_OK SPP", 11);
                        if(Flag == 1)
                        {
                            Connection_SPP_Status = CON_SPP_ON;
                            Read = Write;
                        }
                        else
                        {
                            Read = Write;
                        }
                    }
                    else
                    {
                        Read = Write;
                    }
                }
                else
                {
                    Read = Write;
                }
            }
            else if(Diff == 14)
            {
                Flag = Compaire("CLOSE_OK HFP", 12);
                if(Flag == 1)
                {
                    Connection_HFP_Status = CON_HFP_OFF;
                    Read = Write;
                    BT_Init();
                    SendCmd(Reset, "");
                }
                else if( Flag == 0)
                {
                    Flag = Compaire("CLOSE_OK SPP", 12);
                    if(Flag == 1)
                    {
                        Connection_SPP_Status = CON_SPP_OFF;
                        Read = Write;
                    }
                    else
                    {
                        Read = Write;
                    }
                }
                else
                {
                    Read = Write;
                }
            }
            else if((Diff == 18) || (Diff == 16))
            {
                Flag = Compaire("CALL", 4);
                if(Flag == 1)
                {
                    for(LoopIndexCyclic = 0 ; LoopIndexCyclic < (Diff - 7) ; LoopIndexCyclic++)
                    {
                        NewIndex = LoopIndexFix(LoopIndexCyclic);
                        CallingNum[LoopIndexCyclic] = RcvBuff[NewIndex + 5];
                    }
                    CallIn = IN_CALL;
                    Read = Write;
                }
                else
                {
                    Read = Write;
                }
            }
            else if(Diff > 18)
            {
                Flag = Compaire("AVRCP_MEDIA TITLE:", 18);
                if(Flag == 1)
                {
                    for(LoopIndexCyclic = 0 ; LoopIndexCyclic < (Diff - 21) ; LoopIndexCyclic++)
                    {
                        NewIndex = LoopIndexFix(LoopIndexCyclic);
                        MusicTitle[LoopIndexCyclic] = RcvBuff[NewIndex + 19];
                    }
                    for(LoopIndexCyclic = (Diff - 21) ; LoopIndexCyclic < 50 ; LoopIndexCyclic++)
                    {
                        MusicTitle[LoopIndexCyclic] = '\0';
                    }
                    Read = Write;
                }
                else
                {
                    Read = Write;
                }
            }
            else
            {
                Read = Write;
            }

        }
    }
    else
    {

    }
    return;
}
void BT_Tx_Callback(void)
{

}

void BT_Rx_Callback(void)
{
    WriteOld = Write;
    if(Write == 599)
    {
        Write = 0;
    }
    else
    {
        Write++;
    }
    UART_Rx_Init (&RcvBuff[Write], 1, UART_ChannelID_Rx);
}
static uint16_t LoopIndexFix(uint8_t LoopIndex)
{
    uint16_t NewIndex;
    if(Read + LoopIndex >= RBuff)
    {
        NewIndex = -RBuff + Read + LoopIndex;
    }
    else
    {
        NewIndex = Read + LoopIndex;
    }
    return NewIndex;
}
static uint8_t Difference()
{
    uint8_t Diff;
    if(Read <= Write)
    {
        Diff = Write - Read;
    }
    else
    {
        Diff = RBuff - Read + Write;
    }
    return Diff;
}
static uint8_t Compaire(uint8_t CheckTxt[], uint8_t CompairLen)
{
    uint8_t LoopIndex;
    uint8_t Flag = 1;
    uint16_t NewIndex;
    for(LoopIndex = 0; LoopIndex < CompairLen; LoopIndex++)
    {
        NewIndex = LoopIndexFix(LoopIndex);
        if(CheckTxt[LoopIndex] != RcvBuff[NewIndex])
        {
            Flag = 0;
            LoopIndex = CompairLen;
        }
        else
        {

        }
    }
    return Flag;
}
void SendCmd(uint8_t CmdID, uint8_t Paramiter[])
{
    uint16_t len = 0;
    uint64_t i = 0;
    static uint8_t CallCmdBuff[20] = "CALL ";
    static uint8_t DataCmdBuff[50];

    if(CmdID == BT_Call)
    {
        for(i = 0 ; i < 20 ; i++)
        {
            if((Paramiter[i] !='\0'))
            {
                CallCmdBuff[i + BtCmd[CmdID].CmdLen] = Paramiter[i];
                len = len+1;
            }
            else
            {
                CallCmdBuff[i + BtCmd[CmdID].CmdLen] = '\r';
                len = len+1;
                i = 20;
            }
        }
        UART_Tx_Init(CallCmdBuff, (len + BtCmd[CmdID].CmdLen), UART_ChannelID_Tx);

    }
    else if(CmdID == BT_Data)
    {
        for(i=0 ; i < 50 ; i++)
        {
            if((Paramiter[i] !='\0'))
            {
                DataCmdBuff[i + BtCmd[CmdID].CmdLen] = Paramiter[i];
                len = len+1;
            }
            else
            {
                DataCmdBuff[i + BtCmd[CmdID].CmdLen] = '\r';
                len = len+1;
                i = 50;
            }
        }
        UART_Tx_Init(DataCmdBuff, len, UART_ChannelID_Tx);

    }
    else
    {
        UART_Tx_Init(BtCmd[CmdID].Cmd, BtCmd[CmdID].CmdLen, UART_ChannelID_Tx);
        //for(i=0;i<8500;i++);
        xx=1;
//        UART_Tx(UART_ChannelID_Tx);
//        for(i=0;i<8500;i++);
    }
}
