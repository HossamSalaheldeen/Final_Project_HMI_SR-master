#include "Callback.h"

uint8_t data_frame_sent[MAX_FRAME_LENGTH];
uint8_t data_frame_received[MAX_FRAME_LENGTH]= {0};

uint16_t Data_sent_checksum = 0, Data_received_checksum = 0;
uint16_t received_Pic_ID = 0;

void TxDone_Func(void)
{

}

void RxDone_Func(void)
{

}

void HMI_Tx_Callback (void)
{
    uint8_t HMI_groupID = 0;
    const HMI_ConfigType* CfgPtr;
    CfgPtr = &HMI_ConfigParam[HMI_groupID];
    if(request_state[HMI_groupID] == GETVERSION_REQ)
    {
        UART_Rx_Init(data_frame_received,7,CfgPtr->UART_ChannelID);

    }
    else if(request_state[HMI_groupID] == SELECTPIC_REQ)
    {

        if(HMI_SelectPicture_Func_counter == 0)
        {
            uint8_t checking_frame[MAX_FRAME_LENGTH];
            checking_frame[0] = CfgPtr->frame_header_1st_byte;
            checking_frame[1] = CfgPtr->frame_header_2nd_byte;
            checking_frame[2] = 0x03;
            checking_frame[3] = 0x81;
            checking_frame[4] = 0x03;
            checking_frame[5] = 0x02;

            HMI_SelectPicture_Func_counter++;
            UART_Tx_Init(checking_frame,6,CfgPtr->UART_ChannelID);

        }
        else if(HMI_SelectPicture_Func_counter == 1)
        {

            UART_Rx_Init(data_frame_received,8,CfgPtr->UART_ChannelID);

        }
    }

    else if(request_state[HMI_groupID] == GETPICID_REQ)
    {
        UART_Rx_Init(data_frame_received,8,CfgPtr->UART_ChannelID);

    }
    else if(request_state[HMI_groupID] == SETDATAVAR_REQ)
    {

        if(HMI_SetDataVar_Func_counter == 0)
        {
            uint8_t checking_frame[MAX_FRAME_LENGTH];
            checking_frame[0] = CfgPtr->frame_header_1st_byte;
            checking_frame[1] = CfgPtr->frame_header_2nd_byte;
            checking_frame[2] = 0x04;
            checking_frame[3] = 0x83;
            checking_frame[4] = data_frame_sent[4];
            checking_frame[5] = data_frame_sent[5];
            checking_frame[6] = 0x01;

            HMI_SetDataVar_Func_counter++;
            UART_Tx_Init(checking_frame,7,CfgPtr->UART_ChannelID);

        }
        else if(HMI_SetDataVar_Func_counter == 1)
        {

            UART_Rx_Init(data_frame_received,9,CfgPtr->UART_ChannelID);

        }
    }
    else if(request_state[HMI_groupID] == SETTEXT_REQ)
    {

        if(HMI_SetText_Func_counter == 0)
        {
            uint8_t data_length = ((total_frame_length-6)/2);
            static uint8_t checking_frame[MAX_FRAME_LENGTH];
            checking_frame[0] = CfgPtr->frame_header_1st_byte;
            checking_frame[1] = CfgPtr->frame_header_2nd_byte;
            checking_frame[2] = 0x04;
            checking_frame[3] = 0x83;
            checking_frame[4] = data_frame_sent[4];
            checking_frame[5] = data_frame_sent[5];
            checking_frame[6] = data_length;


            UART_Tx_Init(checking_frame,7,CfgPtr->UART_ChannelID);
            HMI_SetText_Func_counter++;

        }
        else if(HMI_SetText_Func_counter == 1)
        {

            UART_Rx_Init(data_frame_received,(total_frame_length+1),CfgPtr->UART_ChannelID);

        }
    }

}

void HMI_Rx_Callback (void)
{
    uint8_t HMI_groupID = 0;
    const HMI_ConfigType* CfgPtr;
    CfgPtr =&HMI_ConfigParam[HMI_groupID];
    if(request_state[HMI_groupID] == GETVERSION_REQ)
    {

        uint8_t received_version = data_frame_received[6];
        if(received_version == HMI_VERSION)
        {
            CfgPtr->GetVersion_Callback_ptr(HMI_Callback_OK);
        }
        else
        {
            CfgPtr->GetVersion_Callback_ptr(HMI_Callback_NOK);
        }

    }
    else if(request_state[HMI_groupID] == SELECTPIC_REQ)
    {
        if((data_frame_received[6]==data_frame_sent[5])&& (data_frame_received[7]==data_frame_sent[6]))
        {
            CfgPtr->SelectPicture_Callback_ptr(HMI_Callback_OK);
        }
        else
        {
            CfgPtr->SelectPicture_Callback_ptr(HMI_Callback_NOK);
        }
    }
    else if(request_state[HMI_groupID] == GETPICID_REQ)
    {
        uint8_t high_byte_received = data_frame_received[6];
        uint8_t low_byte_received = data_frame_received[7];
        uint16_t Temp1 = (uint16_t)(high_byte_received);
        received_Pic_ID = ((Temp1 << 8) | low_byte_received);

        CfgPtr->GetPictureID_Callback_ptr(HMI_Callback_OK);
    }
    else if(request_state[HMI_groupID] == SETDATAVAR_REQ)
    {
        if((data_frame_received[7]==data_frame_sent[6])&& (data_frame_received[8]==data_frame_sent[7]))
        {
           CfgPtr->SetDataVariable_Callback_ptr(HMI_Callback_OK);
        }
        else
        {
            CfgPtr->SetDataVariable_Callback_ptr(HMI_Callback_NOK);
        }
    }
    else if(request_state[HMI_groupID] == SETTEXT_REQ)
    {
        uint8_t LoopIndex = 0;
        uint8_t data_length = ((total_frame_length-6)/2);

        for(LoopIndex = 0; LoopIndex < total_frame_length; LoopIndex++)
        {
            Data_sent_checksum += data_frame_sent[LoopIndex];
        }

        for(LoopIndex = 0; LoopIndex < (total_frame_length+1); LoopIndex++)
        {
            Data_received_checksum += data_frame_received[LoopIndex];
        }

        Data_received_checksum = Data_received_checksum - ((uint16_t)(data_length + 2));
        if (Data_sent_checksum == Data_received_checksum)
        {
            CfgPtr->SetText_Callback_ptr(HMI_Callback_OK);
        }
        else
        {
            CfgPtr->SetText_Callback_ptr(HMI_Callback_NOK);
        }



    }

    request_state[HMI_groupID] = IDLE;
}
