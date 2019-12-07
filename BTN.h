#ifndef BTN_H_
#define BTN_H_

#include <stdint.h>
#include "BTN_Cfg.h"

typedef enum {BTN_OFF, BTN_JPRSD, BTN_JRLSD, BTN_ON} BtnStates;
typedef enum {ACTIVE_LOW, ACTIVE_HIGH} BtnConn;

typedef struct
{
    uint8_t BtnId;
    BtnConn ConnType;
}BtnConfigType;

extern const BtnConfigType BtnConfigData[NUM_OF_BTNS];

void BTN_Init(void);
void BTN_GetBtnState(uint8_t u8BtnId, uint8_t* pu8BtnStatePtr);

#endif /* BTN_H_ */
