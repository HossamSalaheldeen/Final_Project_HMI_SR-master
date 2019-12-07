#ifndef ALARM_H_
#define ALARM_H_

#include <stdint.h>
#include "Alarm_Cfg.h"

typedef enum{Alarm_OK = 0, Alarm_NOK = 1} Alarm_ChkType;

extern uint8_t Alarm_var;

Alarm_ChkType Alarm_Init (void);
Alarm_ChkType Alarm_Managing_Function (void);

#endif /* ALARM_H_ */
