#include "UART.h"
#include "Callback.h"
#include "PiMngr.h"
#include "BT.h"
#include "GPS.h"

const UART_ConfigType UART_ConfigParam[UART_GROUPS_NUMBER] =
{
    {

    0x00,
    0,
    1,
    9600,
    Regular_Speed,
    OneStopBit,
    Data_8,
    Disabled,
    Enabled,
    16,
    16,
    &PiMngr_TxDone,
    &PiMngr_RxDone,
    Enabled,
    FIFO_2

    },
    {

    0x01,
    2,
    3,
    9600,
    Regular_Speed,
    OneStopBit,
    Data_8,
    Disabled,
    Enabled,
    16,
    16,
    &HMI_Tx_Callback,
    &HMI_Rx_Callback,
    Enabled,
    FIFO_2

    },
    {

    0x02,
    4,
    5,
    9600,
    Regular_Speed,
    OneStopBit,
    Data_8,
    Disabled,
    Enabled,
    16,
    16,
    &TxDone_Func,
    &GPS_FrameRcvd,
    Disabled,
    FIFO_8

    },
    {

    0x03,
    6,
    7,
    9600,
    Regular_Speed,
    OneStopBit,
    Data_8,
    Disabled,
    Enabled,
    16,
    16,
    &BT_Tx_Callback,
    &RxDone_Func,
    Enabled,
    FIFO_2

    },
    {

    0x04,
    8,
    9,
    9600,
    Regular_Speed,
    OneStopBit,
    Data_8,
    Disabled,
    Enabled,
    16,
    16,
    &PiMngr_TxDone,
    &PiMngr_RxDone,
    Enabled,
    FIFO_2

    },
    {

    0x05,
    10,
    11,
    9600,
    Regular_Speed,
    OneStopBit,
    Data_8,
    Disabled,
    Enabled,
    16,
    16,
    &HMI_Tx_Callback,
    &HMI_Rx_Callback,
    Enabled,
    FIFO_2

    },
    {

    0x06,
    12,
    13,
    9600,
    Regular_Speed,
    OneStopBit,
    Data_8,
    Disabled,
    Enabled,
    16,
    16,
    &TxDone_Func,
    &RxDone_Func,
    Enabled,
    FIFO_8

    },
    {

    0x07,
    14,
    15,
    9600,
    Regular_Speed,
    OneStopBit,
    Data_8,
    Disabled,
    Enabled,
    16,
    16,
    &TxDone_Func,
    &BT_Rx_Callback,
    Disabled,
    FIFO_2

    }
};


const GPIO_AlternFunc UART_AlternFunc[UART_CHANNELS_NUMBER]
={

 {0x01,0x01}, //UART0
 {0x01,0x01}, //UART1
 {0x01,0x01}, //UART2
 {0x01,0x01}, //UART3
 {0x01,0x01}, //UART4
 {0x08,0x08}, //UART5
 {0x01,0x01}, //UART6
 {0x01,0x01}  //UART7

};
