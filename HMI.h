#ifndef HMI_H
#define HMI_H

#include <stdint.h>
#include "HMI_Cfg.h"

#define HMI_VERSION 0x71


#define IDLE            10U
#define GETVERSION_REQ  11U
#define SELECTPIC_REQ   12U
#define GETPICID_REQ    13U
#define SETDATAVAR_REQ  14U
#define SETTEXT_REQ     15U

typedef enum {HMI_OK = 0, HMI_NOK = 1, HMI_Busy = 2} HMI_ChkType;
typedef enum {HMI_Callback_OK = 11, HMI_Callback_NOK = 88} HMI_Callback_ChkType;

typedef void (*HMI_PtrToCallbackType) (HMI_Callback_ChkType RetVar);

typedef struct
{

    uint8_t UART_ChannelID;
    uint8_t frame_header_1st_byte;
    uint8_t frame_header_2nd_byte;

    HMI_PtrToCallbackType GetVersion_Callback_ptr;
    HMI_PtrToCallbackType SelectPicture_Callback_ptr;
    HMI_PtrToCallbackType GetPictureID_Callback_ptr;
    HMI_PtrToCallbackType SetDataVariable_Callback_ptr;
    HMI_PtrToCallbackType SetText_Callback_ptr;


} HMI_ConfigType;

extern const HMI_ConfigType HMI_ConfigParam[HMI_CHANNELS_NUMBER];

extern uint8_t HMI_SelectPicture_Func_counter;
extern uint8_t HMI_SetDataVar_Func_counter;
extern uint8_t HMI_SetText_Func_counter;

uint8_t total_frame_length;


HMI_ChkType HMI_Init(uint8_t HMI_groupID);
HMI_ChkType HMI_GetVersion(uint8_t HMI_groupID);
HMI_ChkType HMI_SelectPicture(uint8_t HMI_groupID, uint16_t Pic_ID);
HMI_ChkType HMI_GetPictureID(uint8_t HMI_groupID);
HMI_ChkType HMI_SetDataVariable(uint8_t HMI_groupID, uint16_t Var_ptr, uint16_t Data);
HMI_ChkType HMI_SetText(uint8_t HMI_groupID, uint16_t Var_ptr, uint8_t* string_ptr);


#endif
