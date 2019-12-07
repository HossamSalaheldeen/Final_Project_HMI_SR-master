/*
 * LED.c
 *      Author: Abanob magdy
 */
#include <stdint.h>
#include "led_confg.h"
#include "LED.h"
#include "GPIO.h"
/*
#define LED_STATE_OFF 	0X00
#define LED_STATE_ON 	0X01
#define LED_STATE_BLINK 0X02

#define STACK_SIZE 60

static void Blink(void *pvParameters);

//static TaskHandle_t LED_Tasks [LED_NUMBER] ;                                      // store data of task
static uint8_t LED_State [LED_NUMBER] ={0};
*/

static void Delay(uint8_t GroupID);


LED_Chktype LED_Blink(uint8_t GroupID, uint16_t BlinkTimes)
{
	LED_Chktype RetVar;
	uint8_t LoopIndex;
	if(GroupID < LED_NUMBER)
	{
        for(LoopIndex = 0; LoopIndex < BlinkTimes; LoopIndex++)
        {
            if(LED_ConfigParam[GroupID].ConnectionType == LED_ACTIVE_HIGH)
            {
                GPIO_Write(LED_ConfigParam[GroupID].PinID, 0xFF);
                Delay(GroupID);
                GPIO_Write(LED_ConfigParam[GroupID].PinID, 0x00);
                Delay(GroupID);
            }
            else
            {
                GPIO_Write(LED_ConfigParam[GroupID].PinID, 0x00);
                Delay(GroupID);
                GPIO_Write(LED_ConfigParam[GroupID].PinID, 0xFF);
                Delay(GroupID);
            }
        }
        RetVar = LED_OK;
	}
	else
	{
		RetVar = LED_NOK;
	}
	return RetVar;
}
/*
static void Blink(void *pvParameters)
{
	uint8_t Data=0;
	uint16_t Led;
	uint16_t time;
	time = ((uint16_t*)pvParameters)[1];
	Led = ((uint16_t*)pvParameters)[0];
	while(1)
	{
		GPIO_Write(Led,Data);
		Data ^=0xFF;
		//vTaskDelay(time);
	}
}
*/
LED_Chktype LED_On(uint8_t GroupID)
{
	LED_Chktype RetVar;
	if(GroupID < LED_NUMBER)
	{
		if(LED_ConfigParam[GroupID].ConnectionType == LED_ACTIVE_HIGH)
        {
            GPIO_Write(LED_ConfigParam[GroupID].PinID, 0xFF);
        }
        else
        {
            GPIO_Write(LED_ConfigParam[GroupID].PinID, 0x00);
        }
        RetVar = LED_OK;
	}
	else
	{
		RetVar = LED_NOK;
	}
	return RetVar;
}

LED_Chktype LED_Off(uint8_t GroupID)
{
	LED_Chktype RetVar;
	if(GroupID < LED_NUMBER)
	{
		if(LED_ConfigParam[GroupID].ConnectionType == LED_ACTIVE_HIGH)
        {
            GPIO_Write(LED_ConfigParam[GroupID].PinID, 0x00);
        }
        else
        {
            GPIO_Write(LED_ConfigParam[GroupID].PinID, 0xFF);
        }
        RetVar = LED_OK;
	}
	else
	{
		RetVar = LED_NOK;
	}
	return RetVar;
}


static void Delay(uint8_t GroupID)
{
    uint32_t Index;
    for(Index = 0; Index < LED_ConfigParam[GroupID].Frequency; Index++);
}
