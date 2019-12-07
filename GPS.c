#include <stdint.h>
#include "M4MemMap.h"
#include "GPIO.h"
#include "GPIO_Cfg.h"
#include "UART.h"
#include "UART_Cfg.h"
#include "GPS.h"
#include "GPS_Cfg.h"
#include "GPS_CallBack.h"


#define Requested           1
#define NotRequested        0
#define Number_of_Bytes     500
#define Done                1
#define InitialState        0

#define OneStep             1
#define TwoSteps            2
#define ThreeSteps          3
#define FourSteps           4
#define FiveSteps           5

#define SteppingOneField    1
#define SteppingTwoField    2
#define SteppingNineField   9

#define Tenths              0
#define Units               1


/* The following is an array to save the frame that GPS is sending it */
uint8_t Frame [Number_of_Bytes];
/* The following variables are Flag to indicate that frame is received */
static uint8_t FrameCoordinatesRcvd;
static uint8_t FrameTimeRcvd;
static uint8_t FrameDateRcvd;

static uint16_t GPS_GetRMCframe(void);
static uint16_t GPS_GetField(uint8_t Count, uint16_t LoopIndex);

void GPS_Init(void)
{
    /* Initializing the code first as the user has not requested the data yet and GPS state is idle
       and no frame has been received yet */
    Request = NotRequested;
    GPS_State = GPS_Idle;
    SelfTest = GPS_RequestSelfTest;
    FrameCoordinatesRcvd = InitialState;
    FrameTimeRcvd = InitialState;
    FrameDateRcvd = InitialState;
}

void GPS_Request(void)
{
    /* The aim of this function is to send a request to GPS for sending data */
    Request = Requested;
    UART_Rx_Init(Frame, Number_of_Bytes , GPS_Parameters.UART_ChannelID);
}

void GPS_Cyclic(void)
{
    UART_Rx(GPS_Parameters.UART_ChannelID);
}


void GPS_GetCoordinates(uint8_t* Coordinates)
{
    uint16_t LoopIndex;
    uint8_t LoopIndex1 = InitialState;
    uint8_t LoopIndex2 = 0;
//    static uint8_t Data[23] = "3112.48594,02955.56413";
    static uint8_t Data[23] = "invalid               ";


    uint8_t i;
    /* Checking if the frame is received or not */
    if(FrameCoordinatesRcvd == Done)
    {
        //GPIO_Write(16,0xff);
        if(GPS_State == GPS_Idle)
        {
            if(Request == Requested)
            {
                /* Finding the LoopIndex of $GPRMC field in the frame */
                LoopIndex = GPS_GetRMCframe();
                /* We are looking for the second field in frame $GPRMC */
                LoopIndex = GPS_GetField(SteppingTwoField, LoopIndex);
                if(Frame[LoopIndex] == 'A' )
                {
                    /* If we found the second field in $GPRMC as 'A' which means GPS is ready to send data */
                    GPS_State = GPS_Ready;
                }
                else
                {
                    /* The following line in the code means that we have found the second field as 'S'
                      which means invalid or GPS is not ready to send data */
                    GPS_State = GPS_Idle;
                    for(i = 0; i < 22; i++)
                    {
                        *(Coordinates + i) = Data[i];
                    }
                    /*fabraaka*/
                    GPS_Parameters.GPS_RxCallBack();

                }
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
        /* This part of code to find longitude and latitude value if GPS is ready */
        if(GPS_State == GPS_Ready)
        {
            LoopIndex = LoopIndex + TwoSteps ;
            /* The following part of this code is getting the value of Latitude field in $GPRMC and saving it */
            while(Frame[LoopIndex + LoopIndex1] != ',')
            {
                *(Coordinates + LoopIndex1) = Frame[LoopIndex + LoopIndex1];
                LoopIndex1++;
            }
            //LoopIndex1 = InitialState;
            *(Coordinates + LoopIndex1) = ',';
            LoopIndex1 ++;
            /* The following part of this code we are looking for Longitude starting Index inside the frame */
            LoopIndex = GPS_GetField(SteppingTwoField, LoopIndex);
            //LoopIndex++;
            /* The following part of this code is getting the value of Longitude field in $GPRMC and saving it */
            while(Frame[LoopIndex + LoopIndex2] != ',')
            {
                *(Coordinates + LoopIndex1) = Frame[LoopIndex + LoopIndex2];
                LoopIndex1++;
                LoopIndex2++;
            }
            LoopIndex1 = InitialState;
            GPS_State = GPS_Idle;
            GPS_Parameters.GPS_RxCallBack();
        }
        else
        {
            /* Do nothing */
        }
        FrameCoordinatesRcvd = InitialState;
    }
    else
    {
        /* Do nothing */
    }
}

void GPS_GetTime(uint8_t* Hours, uint8_t* Minutes, uint8_t* Seconds)
{
    uint16_t LoopIndex;
    /* Checking if the frame is received or not */
    if(FrameTimeRcvd == Done)
        {
            if(GPS_State == GPS_Idle)
            {
                if(Request == Requested)
                {
                    /* Finding the starting index of $GPRMC frame inside the array */
                    LoopIndex = GPS_GetRMCframe();
                    /* We are looking for the starting index of time field inside the frame */
                    LoopIndex = GPS_GetField(SteppingOneField, LoopIndex);
                    /* Saving the value of time inside an array */
                    *(Hours + Tenths) = Frame[LoopIndex];
                    *(Hours + Units) = Frame[LoopIndex + OneStep];
                    *(Minutes + Tenths) = Frame[LoopIndex + TwoSteps];
                    *(Minutes + Units) = Frame[LoopIndex + ThreeSteps];
                    *(Seconds + Tenths) = Frame[LoopIndex + FourSteps];
                    *(Seconds + Units) = Frame[LoopIndex + FiveSteps];
                    GPS_Parameters.GPS_RxCallBack();
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }
            FrameTimeRcvd = InitialState;
        }

        else
        {
            /* Do nothing */
        }
}

void GPS_GetDate(uint8_t* Day, uint8_t* Month, uint8_t* Year)
{
    uint16_t LoopIndex;
    /* Checking if the frame is received or not */
    if(FrameDateRcvd == Done)
    {
        if(GPS_State == GPS_Idle)
        {
            if(Request == Requested)
            {
                /* Finding the starting index of $GPRMC frame inside the array */
                LoopIndex = GPS_GetRMCframe();
                /* We are looking for the starting index of date field inside the frame */
                LoopIndex = GPS_GetField(SteppingNineField, LoopIndex);
                /* Saving the value of date inside an array */
                *(Day + Tenths) = Frame[LoopIndex];
                *(Day + Units) = Frame[LoopIndex + OneStep];
                *(Month + Tenths) = Frame[LoopIndex + TwoSteps];
                *(Month + Units) = Frame[LoopIndex + ThreeSteps];
                *(Year + Tenths) = Frame[LoopIndex + FourSteps];
                *(Year + Units) = Frame[LoopIndex + FiveSteps];
                GPS_Parameters.GPS_RxCallBack();
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
        FrameDateRcvd = InitialState;
    }
    else
    {
        /* Do nothing */
    }
}

void GPS_FrameRcvd(void)
{
    /* This function aims to set flags that indicate that frame is received by UART */
    FrameCoordinatesRcvd = Done;
    FrameTimeRcvd = Done;
    FrameDateRcvd = Done;
    UART_Rx_Init(Frame, Number_of_Bytes , GPS_Parameters.UART_ChannelID);
}

void GPS_SelfTest(void)
{
    uint16_t LoopIndex;
    if(SelfTest == GPS_RequestSelfTest)
    {
        UART_Rx_Init(Frame, Number_of_Bytes , GPS_Parameters.UART_ChannelID);
        SelfTest = GPS_CheckingSelfTest;
    }
    else
    {
        /* Do nothing */
    }
    if(SelfTest == GPS_CheckingSelfTest)
    {
        if(FrameCoordinatesRcvd == Done)
        {
            for(LoopIndex = 0; LoopIndex < Number_of_Bytes; LoopIndex ++)
            {
                if(Frame [LoopIndex] != '$' )
                {
                    GPS_Parameters.GPS_SelfTestCallBack();
                    LoopIndex = Number_of_Bytes;
                }
                else
                {
                    SelfTest = GPS_SelfTestNOK;
                }
            }
        }
       // else
        //{

       // }
    }
    else
    {
        /* Do nothing */
    }
}

static uint16_t GPS_GetRMCframe(void)
{
    uint16_t LoopIndex;
    uint8_t Flag = InitialState;
    /* Finding $GPRMC frame inside the array */
    for(LoopIndex = InitialState; (LoopIndex < Number_of_Bytes) && (Flag == InitialState); LoopIndex++)
    {
        if(Frame[LoopIndex] == '$')
        {
            if((Frame[LoopIndex + ThreeSteps ] == 'R') && (Frame[LoopIndex + FourSteps ] == 'M') && (Frame[LoopIndex + FiveSteps ] == 'C'))
            {
                Flag = Done;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
    }
    LoopIndex ++;
    return LoopIndex;

}

static uint16_t GPS_GetField(uint8_t Count, uint16_t LoopIndex)
{
    /* This function is to find starting index of a certain field inside the frame */
    while(Count > 0)
    {
        LoopIndex++;
        if(Frame[LoopIndex] == ',')
        {
            Count--;
        }
        else
        {
            /* Do nothing */
        }
    }
    LoopIndex++;
    return LoopIndex;
}



