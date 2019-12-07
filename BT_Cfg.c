#include <stdint.h>
#include "BT_Cfg.h"
#include "BT.h"

const BT_ConfigParam BtCmd[NumOfCmd] =
{
     {"ANSWER\r",7},
     {"REJECT\r",7},
     {"END\r",4},
     {"CALL ",5},
     {"MUSIC PLAY\r",11},
     {"MUSIC PAUSE\r",12},
     {"MUSIC STOP\r",11},
     {"MUSIC FORWARD\r",14},
     {"MUSIC BACKWARD\r",15},
     {"SET AUTOCONN=0\rSET CODEC=7 0 1\rSET NAME=MY_HMI_CAR\rSET CLASSIC_ROLE=0\rDISCOVERABLE ON\rWRITE\rRESET\r",90},
     //{"SET CODEC=7 0 1\r",16},
     //{"SET NAME=MY_HMI_CAR\r",16},
     //{"DISCOVERABLE ON\r",16},
     //{"SET CLASSIC_ROLE=0\r",19},
     //{"WRITE\r",6},
     {"RESET\r",6},
     {"ENTER_DATA\r",11},
     {"",0},
     {"$$$$",4}
};
