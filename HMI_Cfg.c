#include "HMI.h"
#include "HMI_Cfg.h"
#include "HMI_Manager_Callbacks.h"

const HMI_ConfigType HMI_ConfigParam[HMI_CHANNELS_NUMBER] =
{
 {

  1,
  0x5A,
  0xA5,
  &HMI_GetVersion_done,
  &HMI_SelectPicture_done,
  &HMI_GetPictureID_done,
  &HMI_SetDataVariable_done,
  &HMI_SetText_done
 }
};


