#include <stdint.h>
#include "BTN.h"
#include "BTN_Cfg.h"
#include "GPIO.h"


static BtnStates State[NUM_OF_BTNS];


void BTN_Init(void)
{
	uint8_t LoopIndex;
	for(LoopIndex = 0; LoopIndex < NUM_OF_BTNS; LoopIndex++)
	{
	    if(BtnConfigData[LoopIndex].ConnType == ACTIVE_HIGH)
	    {
	        GPIO_Write(BtnConfigData[LoopIndex].BtnId, 0x00);
	    }
	    else
	    {
	        GPIO_Write(BtnConfigData[LoopIndex].BtnId, 0xFF);
	    }
	    State[LoopIndex] = BTN_OFF;
	}
}

void BTN_GetBtnState(uint8_t u8BtnId, uint8_t* pu8BtnStatePtr)
{
	uint8_t Data;
	GPIO_Read(BtnConfigData[u8BtnId].BtnId, &Data);
	switch(State[u8BtnId])
	{
	    case BTN_OFF:
	    {
	        if(Data != 0)
	        {
	            if(BtnConfigData[u8BtnId].ConnType == ACTIVE_HIGH)
	            {
	                State[u8BtnId] = BTN_JPRSD;
	            }
	            else
	            {
	                State[u8BtnId] = BTN_OFF;
	            }
	        }
	        else
	        {
                if(BtnConfigData[u8BtnId].ConnType == ACTIVE_HIGH)
                {
                    State[u8BtnId] = BTN_OFF;
                }
                else
                {
                    State[u8BtnId] = BTN_JPRSD;
                }
	        }
	    }
	    break;
	    case BTN_JPRSD:
	    {
	        if(Data != 0)
	        {
                if(BtnConfigData[u8BtnId].ConnType == ACTIVE_HIGH)
                {
                    State[u8BtnId] = BTN_ON;
                }
                else
                {
                    State[u8BtnId] = BTN_OFF;
                }
	        }
	        else
	        {
                if(BtnConfigData[u8BtnId].ConnType == ACTIVE_HIGH)
                {
                    State[u8BtnId] = BTN_OFF;
                }
                else
                {
                    State[u8BtnId] = BTN_ON;
                }
	        }
	    }
	    break;
	    case BTN_JRLSD:
	    {
	        if(Data != 0)
	        {
                if(BtnConfigData[u8BtnId].ConnType == ACTIVE_HIGH)
                {
                    State[u8BtnId] = BTN_ON;
                }
                else
                {
                    State[u8BtnId] = BTN_OFF;
                }
	        }
	        else
	        {
                if(BtnConfigData[u8BtnId].ConnType == ACTIVE_HIGH)
                {
                    State[u8BtnId] = BTN_OFF;
                }
                else
                {
                    State[u8BtnId] = BTN_ON;
                }
	        }
	    }
	    break;
	    case BTN_ON:
	    {
	        if(Data != 0)
	        {
                if(BtnConfigData[u8BtnId].ConnType == ACTIVE_HIGH)
                {
                    State[u8BtnId] = BTN_ON;
                }
                else
                {
                    State[u8BtnId] = BTN_JRLSD;
                }
	        }
	        else
	        {
                if(BtnConfigData[u8BtnId].ConnType == ACTIVE_HIGH)
                {
                    State[u8BtnId] = BTN_JRLSD;
                }
                else
                {
                    State[u8BtnId] = BTN_ON;
                }
	        }
	    }
	    break;
	}
	*pu8BtnStatePtr = State[u8BtnId];
}
