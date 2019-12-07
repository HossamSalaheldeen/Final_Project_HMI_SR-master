#ifndef CAN_TRIAL_H_
#define CAN_TRIAL_H_

//This function initializes the CAN Bus on Port B
void CAN0_Init();
//Initialize the transmit message object
void CAN0_Transmit_Set(uint8_t objNo);
//Initialize the Receive message object
void CAN0_Receive_Set(uint8_t objNo);
//This function actually sends the data on the TX pin.
//Since registers have already been initialized in the CAN0_Transmit_Set function
void CAN0_Transmit_Data(uint8_t objNo,uint16_t Data);
//This function actually receives the data put on the RX pin.
//Since registers have already been initialized in the CAN0_Receive_Set function
uint16_t CAN0_Receive_Data(uint8_t objNo);




#endif /* CAN_TRIAL_H_ */
