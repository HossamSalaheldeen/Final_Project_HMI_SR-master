#include "GPS_Cfg.h"
#include "GPS.h"
#include "GPS_CallBack.h"

const GPS_CfgParameters GPS_Parameters =
 {
    2,
    &GPS_SelfTestFunc,
    &LED_ON
 };
