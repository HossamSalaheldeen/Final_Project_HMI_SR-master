#include <stdint.h>
#include "led_confg.h"

typedef enum {OFF =0, ON = 1, TOGGLE = 2} LED_Type;

typedef enum {LED_ACTIVE_HIGH =0 , LED_ACTIVE_LOW} LED_ConnectionType;

typedef enum {LED_OK = 0,LED_NOK = 1 } LED_Chktype;

typedef struct
{
	uint8_t PinID;
    LED_ConnectionType ConnectionType;
	uint32_t Frequency;
}LED_ConfigType;

extern const LED_ConfigType LED_ConfigParam[LED_NUMBER];

LED_Chktype LED_On(uint8_t GroupID);
LED_Chktype LED_Off(uint8_t GroupID);
LED_Chktype LED_Blink(uint8_t GroupID, uint16_t BlinkTimes);




