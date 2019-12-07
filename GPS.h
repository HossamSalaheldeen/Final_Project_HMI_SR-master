#include <stdint.h>

#ifndef GPS_H_
#define GPS_H_

typedef enum {GPS_OK =0, GPS_NOK} GPS_CheckType;
typedef enum {GPS_Idle =0, GPS_Ready, GPS_Wait, GPS_Error} enum_GPS_State;
typedef enum {GPS_RequestSelfTest,GPS_CheckingSelfTest,GPS_SelfTestOK, GPS_SelfTestNOK} GPS_CheckTest;
typedef void(*GPS_PtrToCallbackType)(void);

typedef struct
{
    uint8_t UART_ChannelID;
    GPS_PtrToCallbackType GPS_SelfTestCallBack;
    GPS_PtrToCallbackType GPS_RxCallBack;
}GPS_CfgParameters;


extern const GPS_CfgParameters GPS_Parameters;

/* The following two variables Request and State are used to say if the user requested data of not
and to say if GPS is ready to send data or not */
uint8_t Request;
enum_GPS_State GPS_State;
GPS_CheckTest SelfTest;
//


/* Functions declaration */
void GPS_Init(void);
void GPS_Request(void);
void GPS_Cyclic(void);
void GPS_GetCoordinates(uint8_t* Coordinates);
void GPS_GetTime(uint8_t* Hours, uint8_t* Minutes, uint8_t* Seconds);
void GPS_GetDate(uint8_t* Day, uint8_t* Month, uint8_t* Year);
void GPS_FrameRcvd(void);
void GPS_SelfTest(void);
//


#endif /* GPS_H_ */
