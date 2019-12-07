#ifndef PIMNGR_H_
#define PIMNGR_H_

typedef enum {Unpressed = 1, Initial , ReadytoRecognize, ReadytoReceive, BusytoRecognize, BusytoReceive, ReceptionDone, Done, InitialInProg, ReadytoRecognizeInProg, ReceptionInProg, Doneeee} PiStates;
typedef enum {Pi_Navigation = 1, Pi_Music, Pi_Play, Pi_Stop, Pi_Next, Pi_Previous, Pi_Call, Pi_Answer, Pi_Reject, Pi_Finish, Pi_Engine, Pi_Daniel, Pi_Mark, Pi_Jessica, Pi_Error} Word;

uint8_t ID;
uint8_t DataLen;
uint8_t Data[18];

extern uint8_t RxBuff[20];
extern uint8_t RxBuff_Word[20];
extern PiStates State;
extern uint8_t RecognitionDone;

void PiMngr_GetStatus(void);
void PiMngr_StartRecognition(void);
void PiMngr_RequestData(void);

void PiMngr_Callback_Func(void);

void PiMngr_TxDone(void);
void PiMngr_RxDone(void);

#endif /* PIMNGR_H_ */
