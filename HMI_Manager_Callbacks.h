#ifndef HMI_MANAGER_CALLBACKS_H
#define HMI_MANAGER_CALLBACKS_H

#include <stdint.h>
#include "HMI.h"
#include "HMIManager.h"
#include "HMIManager_Cfg.h"

extern HMI_Callback_ChkType HMI_GetVersion_Callback_Flag;
extern HMI_Callback_ChkType HMI_SelectPicture_Callback_Flag;
extern HMI_Callback_ChkType HMI_GetPictureID_Callback_Flag;
extern HMI_Callback_ChkType HMI_SetDataVariable_Callback_Flag;
extern HMI_Callback_ChkType HMI_SetText_Callback_Flag;

void HMI_GetVersion_done(HMI_Callback_ChkType retVar);
void HMI_SelectPicture_done(HMI_Callback_ChkType retVar);
void HMI_GetPictureID_done(HMI_Callback_ChkType retVar);
void HMI_SetDataVariable_done(HMI_Callback_ChkType retVar);
void HMI_SetText_done(HMI_Callback_ChkType retVar);

#endif
