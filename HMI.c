#include "HMI.h"
#include "HMI_Cfg.h"
#include "UART.h"
#include "Callback.h"


uint8_t HMI_SelectPicture_Func_counter;
uint8_t HMI_SetDataVar_Func_counter;
uint8_t HMI_SetText_Func_counter;

uint8_t request_state[HMI_CHANNELS_NUMBER];



HMI_ChkType HMI_Init(uint8_t HMI_groupID)
{
    HMI_ChkType RetVar;
    request_state[HMI_groupID] = IDLE;


    RetVar = HMI_OK;

    return RetVar;
}

HMI_ChkType HMI_GetVersion(uint8_t HMI_groupID)
{

    HMI_ChkType RetVar;

    if(HMI_groupID < HMI_CHANNELS_NUMBER)
    {

    if (request_state[HMI_groupID] == IDLE)
    {
        request_state[HMI_groupID] = GETVERSION_REQ;
        const HMI_ConfigType* CfgPtr;
        CfgPtr = &HMI_ConfigParam[HMI_groupID];
        data_frame_sent[0] = CfgPtr->frame_header_1st_byte;
        data_frame_sent[1] = CfgPtr->frame_header_2nd_byte;
        data_frame_sent[2] = 0x03;
        data_frame_sent[3] = 0x81;
        data_frame_sent[4] = 0x00;
        data_frame_sent[5] = 0x01;

        UART_Tx_Init(data_frame_sent,6,CfgPtr->UART_ChannelID);


        RetVar = HMI_OK;

    }
    else
    {
        RetVar = HMI_Busy;
    }


    }
    else
    {
        RetVar = HMI_NOK;
    }

    return RetVar;

}

HMI_ChkType HMI_SelectPicture(uint8_t HMI_groupID, uint16_t Pic_ID)
{


    HMI_ChkType RetVar;

    if(HMI_groupID < HMI_CHANNELS_NUMBER)
    {

    if (request_state[HMI_groupID] == IDLE)
    {
        uint16_t Temp = Pic_ID;
        uint8_t high_byte = (uint8_t)(Pic_ID >> 8);
        uint8_t low_byte = (uint8_t)(Temp & 0x00FF);


        request_state[HMI_groupID] = SELECTPIC_REQ;
        const HMI_ConfigType* CfgPtr;
        CfgPtr = &HMI_ConfigParam[HMI_groupID];
        data_frame_sent[0] = CfgPtr->frame_header_1st_byte;
        data_frame_sent[1] = CfgPtr->frame_header_2nd_byte;
        data_frame_sent[2] = 0x04;
        data_frame_sent[3] = 0x80;
        data_frame_sent[4] = 0x03;
        data_frame_sent[5] = high_byte;
        data_frame_sent[6] = low_byte;

        UART_Tx_Init(data_frame_sent,7,CfgPtr->UART_ChannelID);
        HMI_SelectPicture_Func_counter = 0;


        RetVar = HMI_OK;

    }
    else
    {
        RetVar = HMI_Busy;
    }


    }
    else
    {
        RetVar = HMI_NOK;
    }

    return RetVar;
}

HMI_ChkType HMI_GetPictureID(uint8_t HMI_groupID)
{

    HMI_ChkType RetVar;

    if(HMI_groupID < HMI_CHANNELS_NUMBER)
    {

    if (request_state[HMI_groupID] == IDLE)
    {
        request_state[HMI_groupID] = GETPICID_REQ;
        const HMI_ConfigType* CfgPtr;
        CfgPtr = &HMI_ConfigParam[HMI_groupID];
        data_frame_sent[0] = CfgPtr->frame_header_1st_byte;
        data_frame_sent[1] = CfgPtr->frame_header_2nd_byte;
        data_frame_sent[2] = 0x03;
        data_frame_sent[3] = 0x81;
        data_frame_sent[4] = 0x03;
        data_frame_sent[5] = 0x02;

        UART_Tx_Init(data_frame_sent,6,CfgPtr->UART_ChannelID);

        RetVar = HMI_OK;

    }
    else
    {
        RetVar = HMI_Busy;
    }


    }
    else
    {
        RetVar = HMI_NOK;
    }

    return RetVar;
}

HMI_ChkType HMI_SetDataVariable(uint8_t HMI_groupID, uint16_t Var_ptr, uint16_t Data)
{

    HMI_ChkType RetVar;

    if(HMI_groupID < HMI_CHANNELS_NUMBER)
    {

    if (request_state[HMI_groupID] == IDLE)
    {

        uint16_t Temp1 = Var_ptr;
        uint8_t var_ptr_high_byte = (uint8_t)(Var_ptr >> 8);
        uint8_t var_ptr_low_byte = (uint8_t)(Temp1 & 0x00FF);


        uint16_t Temp2 = Data;
        uint8_t data_high_byte = (uint8_t)(Data >> 8);
        uint8_t data_low_byte = (uint8_t)(Temp2 & 0x00FF);


        request_state[HMI_groupID] = SETDATAVAR_REQ;
        const HMI_ConfigType* CfgPtr;
        CfgPtr = &HMI_ConfigParam[HMI_groupID];
        data_frame_sent[0] = CfgPtr->frame_header_1st_byte;
        data_frame_sent[1] = CfgPtr->frame_header_2nd_byte;
        data_frame_sent[2] = 0x05;
        data_frame_sent[3] = 0x82;
        data_frame_sent[4] = var_ptr_high_byte;
        data_frame_sent[5] = var_ptr_low_byte;
        data_frame_sent[6] = data_high_byte;
        data_frame_sent[7] = data_low_byte;

        UART_Tx_Init(data_frame_sent,8,CfgPtr->UART_ChannelID);
        HMI_SetDataVar_Func_counter = 0;

        RetVar = HMI_OK;

    }
    else
    {
        RetVar = HMI_Busy;
    }


    }
    else
    {
        RetVar = HMI_NOK;
    }

    return RetVar;

}

HMI_ChkType HMI_SetText(uint8_t HMI_groupID, uint16_t Var_ptr, uint8_t* string_ptr)
{
    HMI_ChkType RetVar;
    uint8_t string_length = 0;
    while((*(string_ptr)) != '\0')
    {
        string_length++;
        string_ptr++;
    }
    string_ptr = string_ptr - string_length;
    uint8_t frame_length_to_be_sent = (string_length * 2) + 3;
    total_frame_length = frame_length_to_be_sent + 3;



    if(HMI_groupID < HMI_CHANNELS_NUMBER)
    {
        if (request_state[HMI_groupID] == IDLE)
        {

            if(string_length <= ((MAX_FRAME_LENGTH - 6)/2))
            {

                uint16_t Temp1 = Var_ptr;
                uint8_t var_ptr_high_byte = (uint8_t)(Var_ptr >> 8);
                uint8_t var_ptr_low_byte = (uint8_t)(Temp1 & 0x00FF);

                request_state[HMI_groupID] = SETTEXT_REQ;
                const HMI_ConfigType* CfgPtr;
                CfgPtr = &HMI_ConfigParam[HMI_groupID];


                data_frame_sent[0] = CfgPtr->frame_header_1st_byte;
                data_frame_sent[1] = CfgPtr->frame_header_2nd_byte;
                data_frame_sent[2] = frame_length_to_be_sent;
                data_frame_sent[3] = 0x82;
                data_frame_sent[4] = var_ptr_high_byte;
                data_frame_sent[5] = var_ptr_low_byte;


                uint8_t LoopIndex = 0;
                for(LoopIndex = 6; LoopIndex < (frame_length_to_be_sent + 2); LoopIndex++)
                {
                    data_frame_sent[LoopIndex] = 0x00;
                    LoopIndex++;
                    data_frame_sent[LoopIndex] = *(string_ptr);
                    string_ptr++;
                }

                UART_Tx_Init(data_frame_sent,total_frame_length,CfgPtr->UART_ChannelID);
                HMI_SetText_Func_counter = 0;

                RetVar = HMI_OK;


            }
            else
            {
                RetVar = HMI_NOK;
            }
        }
        else
        {
            RetVar = HMI_Busy;
        }

    }
    else
    {
        RetVar = HMI_NOK;
    }

    return RetVar;
}


