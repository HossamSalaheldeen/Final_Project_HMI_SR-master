/*
 * app.h
 *
 *  Created on: Jul 1, 2018
 *      Author: Sector Computer
 */

#ifndef APP_H_
#define APP_H_

#define TIME_OUT_SCREEN     1000
#define HMI_ID              0

#define APP_PI_LED          0U
#define APP_CAN_LED         1U
#define APP_BT_LED          2U
#define APP_GPS_LED         3U
#define APP_HMI_LED         4U

#define RECORDING_START     1
#define RECORDING_END       0

#define ENGINE_STATUS_CHANGE 0XFF

extern uint8_t StateX;
extern uint32_t counter,state;
void app(void);


enum
{
    ReadyToRecognize,RecognitionRequested,EngineHandling,GPSHandling,InCallHandling,
    MusicPlayHandling,MusicStopHandling,NextHandling,PrevHandling,AnswerHandling,RejectHandling,EndHandling,
    Name1Hand,Name2Hand,Name3Hand,qq,ww,qqq,ErrorHandling
}enum_APP_State;


#endif /* APP_H_ */
