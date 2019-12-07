#ifndef CALLBACK_H
#define CALLBACK_H

#include <stdint.h>
#include "GPIO.h"
#include "UART.h"
#include "HMI.h"
#include "HMI_Manager_Callbacks.h"

extern uint8_t request_state[HMI_CHANNELS_NUMBER];
extern uint8_t data_frame_sent[MAX_FRAME_LENGTH];
extern uint8_t data_frame_received[MAX_FRAME_LENGTH];

extern uint16_t received_Pic_ID;


extern uint8_t total_frame_length;

extern uint16_t Data_sent_checksum;
extern uint16_t Data_received_checksum;

void TxDone_Func (void);
void RxDone_Func (void);

void HMI_Tx_Callback (void);
void HMI_Rx_Callback (void);

#endif
