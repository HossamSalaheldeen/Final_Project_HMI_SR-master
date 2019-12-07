#include <Stdint.h>
#include"Project_Includes.h"
#include "UART.h"
#include "UART_Cfg.h"
#include "M4MemMap.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"



/* Define Tasks Priorities */
#define  TASK1_PRIORITY (1)
#define  TASK2_PRIORITY (2)
#define  TASK3_PRIORITY (0)

/*tasks codes prototypes */
 void task1_code(void*pvParamter);
 void task2_code(void*pvParamter);
 void task3_code(void*pvParamter);

/*Global Flags definition\declaration */
extern uint8_t Music_Satus_Flag,Engine_Satus_Flag;
extern uint8_t StateX;
uint8_t CoordinatesData[23];

void main(void)
{
    /*CAN and GPIO Initialization*/
    uint8_t objNo = 1;
    CAN0_Init();
    GPIO_Init();
    CAN0_Transmit_Set(objNo); //from inits
    /*UART Initialization and enabling Interrupts  */
    UART_Init();
    EN_INT(6);  //UART1
    EN_INT(59); //UART3
    EN_INT(60); //UART4
    /*Button initialization*/
    BTN_Init();
    /*Bluetooth initialization*/
    BT_Init();
    SendCmd(Reset,"");
    /*GPS initialization*/
    GPS_Init();
    /*Alarm initialization*/
    Alarm_Init();
    /*Pi Manager initialization*/
    State = Initial;
    /*HMI Screen Initialization and Version Self-Testing */
    HMI_Init(0);
    Manager_Init();
    Manager_Self_Test(0);
    /* Creating tasks */
    xTaskCreate(task1_code,NULL,80,NULL,TASK1_PRIORITY,NULL);
    xTaskCreate(task2_code,NULL,80,NULL,TASK2_PRIORITY,NULL);
    xTaskCreate(task3_code,NULL,80,NULL,TASK3_PRIORITY,NULL);
    /* start Scheduler */
    vTaskStartScheduler();
while(1)
  {

  }
}
 /*Task1 Comes every 5ms*/
  void task1_code(void*pvParamter)
 {
     TickType_t xLastWakeTime;
     xLastWakeTime = xTaskGetTickCount();
     while(1)
     {
         HMI_Managing_Function();
         GPS_Cyclic();
         GPS_GetCoordinates(CoordinatesData);
         vTaskDelayUntil( &xLastWakeTime,10);
     }
 }
  /*Task1 Comes every 10ms*/
  void task2_code(void*pvParamter)
 {
      TickType_t xLastWakeTime;
      xLastWakeTime = xTaskGetTickCount();
      while(1)
      {
          app();
          Alarm_Managing_Function();
          vTaskDelayUntil( &xLastWakeTime,20);
      }
 }
  /*Task1 Comes every 0.5ms*/
  void task3_code(void*pvParamter)
 {
      TickType_t xLastWakeTime;
      xLastWakeTime = xTaskGetTickCount();
      while(1)
      {
          BTN_GetBtnState(0, &StateX);
          BT_Cyclic();
          vTaskDelayUntil( &xLastWakeTime,1);
      }
 }

