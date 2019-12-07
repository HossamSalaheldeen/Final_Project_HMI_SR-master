#ifndef HMI_MANAGER_H
#define HMI_MANAGER_H

#include <stdint.h>
#include "HMIManager_Cfg.h"
#include "PiMngr.h"
/*ENGINE_DELAY is defined as the number of counts to acheive a timer delay .. if manager comes every 10ms
 * so ENGINE_DELAY = 100 to get a 1 second delay on pic  */

#define ENGINE_DELAY 100
#define MUSIC_DELAY  100
#define INCOMINGCALL_DELAY 300
#define BTH_DELAY   100
#define GPS_DELAY 100


typedef enum {HMIManager_OK = 0, HMIManager_NOK = 1} HMIManager_ChkType;
typedef enum {Play , Pause , Previous , Next ,Stop} enum_Music_Status;
typedef enum {Engine_OFF , Engine_ON} enum_Engine_Status;
typedef enum {NoDecision = 0, Answer ,Reject } enum_HMI_CallDecision;

typedef enum {
INIT,
SELF_TEST,
ENGINE_STATUS,
MUSIC_STATUS,
INCOMING_CALL,
BTH_STATUS,
GPS_STATUS,
MUSIC_NAME,
RECOGNITION
} FLAGS;

typedef enum {
    HMI_UNINIT,
    HMI_INIT_DONE,
    HMI_SELF_TEST,
    HMI_READY,
    HMI_BUSY,
    HMI_ERROR,
    HMI_ENGINE1,
    HMI_ENGINE2,
    HMI_ENGINE1_BUSY,
    HMI_ENGINE2_BUSY,
    HMI_INCOMING_CALL1,
    HMI_INCOMING_CALL2,
    HMI_INCOMING_CALL1_BUSY,
    HMI_INCOMING_CALL2_BUSY,
    HMI_MUSIC_STATUS1,
    HMI_MUSIC_STATUS1_BUSY,
    HMI_BTH_STATUS1,
    HMI_BTH_STATUS1_BUSY,
    HMI_GPS_STATUS1,
    HMI_GPS_STATUS2,
    HMI_GPS_STATUS1_BUSY,
    HMI_GPS_STATUS2_BUSY,
    HMI_ERASE_STATE,
    HMI_ERASE_STATE_BUSY
} STATES;

typedef struct
{
    uint16_t HMI_HomeScreen_ID; // picture
    uint16_t HMI_EngineON_ID; // picture
    uint16_t HMI_EngineOFF_ID; // picture
    uint16_t HMI_EngineStatusSetText_ID;// mkan le ketabet ONN/OFF
    uint16_t HMI_BTHStatusConnected_ID;//picture
    uint16_t HMI_BTHStatusDisconnected_ID;//picture
    uint16_t HMI_GPSDisplay_ID;//Picture to display GPS
    uint16_t HMI_ContactNumber_ID;// place to write number in incoming call page
    uint16_t HMI_IncomingCall_ID;//picture
    uint16_t HMI_MusicStatus_ID; // TEXT MUSIC : [PLAYED,PAUSED,PREV,NEXT]
    uint16_t HMI_GPSData_ID; //GPS Data [Longitude + N/S and Latitude + E/W]
    uint16_t HMI_CallPicture_ID;
    uint16_t HMI_CallText_ID;
    uint16_t HMI_RecognitionStarted_ID;
    uint16_t HMI_MusicName_ID;
} HMIManager_ConfigType;

extern const HMIManager_ConfigType HMIManager_ConfigParam[HMIManager_CHANNELS_NUMBER];
extern uint8_t Engine_Status_Flag ,Call_Status_Flag,Music_Status_Flag,BTH_Status_Flag  ;
extern uint16_t HMI_Flag[HMIManager_CHANNELS_NUMBER];
extern STATES HMI_State[HMIManager_CHANNELS_NUMBER] ;
extern uint8_t Global_Loop_Index;


HMIManager_ChkType HMI_Managing_Function(void);
HMIManager_ChkType Manager_Init(void);
HMIManager_ChkType Manager_Self_Test(uint8_t HMI_groupID);
HMIManager_ChkType Engine_Status_Display(uint8_t HMI_groupID , uint8_t Engine_Status); // 0 for off 1 for on
HMIManager_ChkType Music_Status_Display(uint8_t HMI_groupID , uint8_t Music_Status); //Play , Pause , Previous , Next
HMIManager_ChkType Incoming_Call_Display(uint8_t HMI_groupID, uint8_t Number[]  );
HMIManager_ChkType BTH_Status_Display(uint8_t HMI_groupID,uint8_t BTH_Status);//connected - disconnected
HMIManager_ChkType GPS_Status_Display( uint8_t HMI_groupID  , uint8_t Local_GPSData[]);
HMIManager_ChkType HMI_MusicName_Display(uint8_t HMI_groupID  , uint8_t Local_MusicName[] );
HMIManager_ChkType HMI_RecognitionState(uint8_t HMI_groupID  , uint8_t Rec_State ); //1 start 0 end
HMIManager_ChkType HMI_CallDecision (uint8_t HMI_groupID , uint8_t Decision);

#endif
