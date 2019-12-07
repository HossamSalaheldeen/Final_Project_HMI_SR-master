#ifndef BT_H_
#define BT_H_

#include "BT_Cfg.h"
extern uint32_t xx;

typedef enum {Startup, WaitConnection, NeedConfiguration, Restart, WaitResponse} BtStates;
typedef enum {BT_Answer, BT_Reject, BT_End, BT_Call, PlayMusic, PauseMusic, StopMusic, ForwardMusic, BackwardMusic,
                Configuration, Reset, EnterData, BT_Data, ExitData} CmdIndex;
typedef enum {BT_OK, BT_NOK, BT_POWERUP, BT_NPOWERUP} BtStatus;
typedef enum {CMD_OK, CMD_ERROR, CMD_NUTRAL} CmdResponse;
typedef enum {CALL_START, CALL_END, CALL_NUTRAL} CallStatus;
typedef enum {CON_HFP_ON, CON_HFP_OFF, CON_HFP_FAIL} ConnectionHFPStatus;
typedef enum {CON_SPP_ON, CON_SPP_OFF} ConnectionSPPStatus;
typedef enum {IN_CALL, NO_CALL} IncommingCall;

BtStates BT_State;
CmdResponse Response;
CallStatus Call_Status;
ConnectionHFPStatus Connection_HFP_Status;
ConnectionSPPStatus Connection_SPP_Status;
BtStatus Bt_Status;
uint8_t CallingNum[11];
uint8_t MusicTitle[50];
IncommingCall CallIn;

typedef struct
{
    uint8_t *Cmd;
    uint8_t CmdLen;
}BT_ConfigParam;

void BT_Init(void);
void BT_Cyclic(void);
void BT_Tx_Callback(void);
void BT_Rx_Callback(void);
void SendCmd(uint8_t CmdID, uint8_t PhoneNum[]);

extern const BT_ConfigParam BtCmd[NumOfCmd];

#endif
