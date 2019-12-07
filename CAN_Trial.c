#include <stdint.h>
#include "CAN_Trial.h"

typedef volatile uint32_t* const CAN_RegAddType;

#define CAN0BaseAddress     0x40040000
#define CAN1BaseAddress     0x40041000

#define GPIOPortBAddress    0x40005000
#define GPIOPortEAddress    0x40024000
/* Defining Clock Gating Control register 0 */
#define RCGC0           *((CAN_RegAddType)(0x400FE100))

/* Defining Clock Gating Control register for GPIO Port B */
#define RCGC2           *((CAN_RegAddType)(0x400FE108))

/* Defining GPIO PB Initializations Registers */
#define GPIODIR         *((CAN_RegAddType)(GPIOPortEAddress + 0x400))
#define GPIOAFSEL       *((CAN_RegAddType)(GPIOPortEAddress + 0x420))
#define GPIODEN         *((CAN_RegAddType)(GPIOPortEAddress + 0x51C))
#define GPIOCR          *((CAN_RegAddType)(GPIOPortEAddress + 0x524))
#define GPIOAMSEL       *((CAN_RegAddType)(GPIOPortEAddress + 0x528))
#define GPIOPCTL        *((CAN_RegAddType)(GPIOPortEAddress + 0x52C))

/* Defining Control Registers */
#define CANCTL          *((CAN_RegAddType)(CAN0BaseAddress + 0x000))
#define CANSTS          *((CAN_RegAddType)(CAN0BaseAddress + 0x004))
#define CANERR          *((CAN_RegAddType)(CAN0BaseAddress + 0x008))
#define CANBIT          *((CAN_RegAddType)(CAN0BaseAddress + 0x00C))
#define CANINT          *((CAN_RegAddType)(CAN0BaseAddress + 0x010))
#define CANTST          *((CAN_RegAddType)(CAN0BaseAddress + 0x014))
#define CANBRPE         *((CAN_RegAddType)(CAN0BaseAddress + 0x018))

/* Defining CAN0 Interface Registers */
#define CANIF1CRQ       *((CAN_RegAddType)(CAN0BaseAddress + 0x020))
#define CANIF1CMSK      *((CAN_RegAddType)(CAN0BaseAddress + 0x024))
#define CANIF1MSK1      *((CAN_RegAddType)(CAN0BaseAddress + 0x028))
#define CANIF1MSK2      *((CAN_RegAddType)(CAN0BaseAddress + 0x02C))
#define CANIF1ARB1      *((CAN_RegAddType)(CAN0BaseAddress + 0x030))
#define CANIF1ARB2      *((CAN_RegAddType)(CAN0BaseAddress + 0x034))
#define CANIF1MCTL      *((CAN_RegAddType)(CAN0BaseAddress + 0x038))
#define CANIF1DA1       *((CAN_RegAddType)(CAN0BaseAddress + 0x03C))
#define CANIF1DA2       *((CAN_RegAddType)(CAN0BaseAddress + 0x040))
#define CANIF1DB1       *((CAN_RegAddType)(CAN0BaseAddress + 0x044))
#define CANIF1DB2       *((CAN_RegAddType)(CAN0BaseAddress + 0x048))

#define CANIF2CRQ       *((CAN_RegAddType)(CAN0BaseAddress + 0x080))
#define CANIF2CMSK      *((CAN_RegAddType)(CAN0BaseAddress + 0x084))
#define CANIF2MSK1      *((CAN_RegAddType)(CAN0BaseAddress + 0x088))
#define CANIF2MSK2      *((CAN_RegAddType)(CAN0BaseAddress + 0x08C))
#define CANIF2ARB1      *((CAN_RegAddType)(CAN0BaseAddress + 0x090))
#define CANIF2ARB2      *((CAN_RegAddType)(CAN0BaseAddress + 0x094))
#define CANIF2MCTL      *((CAN_RegAddType)(CAN0BaseAddress + 0x098))
#define CANIF2DA1       *((CAN_RegAddType)(CAN0BaseAddress + 0x09C))
#define CANIF2DA2       *((CAN_RegAddType)(CAN0BaseAddress + 0x0A0))
#define CANIF2DB1       *((CAN_RegAddType)(CAN0BaseAddress + 0x0A4))
#define CANIF2DB2       *((CAN_RegAddType)(CAN0BaseAddress + 0x0A8))

/* Defining CAN0 Message Object Registers */
#define CANTXRQ1         *((CAN_RegAddType)(CAN0BaseAddress + 0x100))
#define CANNWDA1         *((CAN_RegAddType)(CAN0BaseAddress + 0x120))
#define CANMSG1INT       *((CAN_RegAddType)(CAN0BaseAddress + 0x140))
#define CANMSG1VAL       *((CAN_RegAddType)(CAN0BaseAddress + 0x160))


static void delay(void)
{
    volatile uint8_t i;
    for(i = 0; i < 10000; i++);
}

void CAN0_Init()
{
    {
        //Port B CAN // PB5 = TX and PB4 = RX
        //Here the Port B is prepared for CAN operation
        volatile unsigned long delay;
        RCGC0 |= 0x01000000;     // Enable Clock for CAN0 Module
        //RCGC2 |= 0x00000002;     // Port B clock gating control
        RCGC2 |= 0x00000010;
        delay = RCGC2;           // delay
        GPIOCR |= 0x30;          // allow changes to PB4-5 //Commit Register
        GPIOAMSEL &= 0x00;       // disable analog function
        GPIODEN |= 0x30;         // Enable Digital Pins
        GPIODIR |= 0x20;         // Direction Output for PB5 and Input for PB4
        GPIOAFSEL |= 0x30;       // CAN0 alternate function on PB4-5
        GPIOPCTL = (GPIOPCTL & 0xFF00FFFF)+0x00880000;
        // Set PCTL Register PMC5 and PMC4 to 0x08 for CAN functionality
    }

    //Initialize CAN0 Module
    {
        //Place CAN0 in Init Mode
        CANCTL |= 0x01;                 //Set Init Bit in the Register
        while(CANIF1CRQ & 0x00008000);  //wait while CAN0 Module is BUSY
    }

    //Enabling Loopback test mode
    //CANCTL |= 0x80;
    //CANTST |= 0x10;

    //Bit rate setting
    {
        CANCTL |= 0x40;     //Setting the CCE bit to edit CANBIT Register
        CANBIT |= 0x0201;   //Setting calculated according to example 1 in datasheet, 16Mhz and 1Mbps = 0x0203
        //But it gave a 500Kbps bit rate soo by trial and error 1Mbps = 0x0201
    }

    //No interrupts are used
    //CAN is enabled (Init = 0)
    CANCTL &= ~0x01;
}

//Initialize the transmit message object
void CAN0_Transmit_Set(uint8_t objNo) //objNo is the Object Number
{
    uint32_t CmdMaskReg = 0;
    uint32_t ArbReg0 = 0;
    uint32_t ArbReg1 = 0;
    uint32_t MsgCtrl = 0;
    uint32_t MaskReg0 = 0;
    uint32_t MaskReg1 = 0;

    while(CANIF1CRQ & 0x00008000);//wait while CAN0 Module is BUSY

    CmdMaskReg |= 0x93; //WRNRD, CONTROL, DATAA, DATAB bits set

    MsgCtrl |= 0x100; //TXRQST bit set (Not Necessary to do this)
    ArbReg1 |= 0x2000; // DIR bit setting in ARB2 reg

    CmdMaskReg |= 0x20; //ARB bit set in CMSK reg
    ArbReg1 |= 0x04; // MSG ID set to 0x01 in ARB2 reg

    ArbReg1 |= 0x8000; //MSG Valid bit set in ARB2 reg
    MsgCtrl |= 0x08; // Set no. of data bytes in MCTL Reg

    CANIF1CMSK = CmdMaskReg; //Registers updation
    CANIF1MSK1 = MaskReg0;
    CANIF1MSK2 = MaskReg1;
    CANIF1ARB1 = ArbReg0;
    CANIF1ARB2 = ArbReg1;
    CANIF1MCTL = MsgCtrl;

    CANIF1CRQ = objNo; //Message object number
}

//Initialize the Receive message object
void CAN0_Receive_Set(uint8_t objNo)
{
    uint32_t CmdMaskReg = 0;
    uint32_t ArbReg0 = 0;
    uint32_t ArbReg1 = 0;
    uint32_t MsgCtrl = 0;
    uint32_t MaskReg0 = 0;
    uint32_t MaskReg1 = 0;

    while(CANIF2CRQ & 0x00008000);//wait while CAN0 Module is BUSY

    CmdMaskReg |= 0xD3; //WRNRD, CONTROL, MASK, DATAA, DATAB bits set

    ArbReg1 |= 0x00; // DIR bit setting in ARB2 reg

    CmdMaskReg |= 0x20; //ARB bit set in CMSK reg
    ArbReg1 |= 0x04;

    ArbReg1 |= 0x8000; //MSG Valid bit set in ARB2 reg
    MsgCtrl |= 0x1088; // Set no. of data bytes in MCTL Reg, EOB Set and UMASK set.

    CANIF2CMSK = CmdMaskReg; //Registers updation
    CANIF2MSK1 = MaskReg0;
    CANIF2MSK2 = MaskReg1;
    CANIF2ARB1 = ArbReg0;
    CANIF2ARB2 = ArbReg1;
    CANIF2MCTL = MsgCtrl;

    CANIF2CRQ = objNo; //Message object number

    while(CANIF2CRQ & 0x00008000);//wait while CAN0 Module is BUSY
}

//This function actually sends the data on the TX pin.
//Since registers have already been initialized in the CAN0_Transmit_Set function
void CAN0_Transmit_Data(uint8_t objNo,uint16_t Data)
{
    CANIF1CMSK &= ~0x30; //Clear ARB and CONTROL bits in CMSK reg
    CANIF1CMSK |= 0x83; //Set WRNRD, DATAA, DATAB bits in CMSK

    CANIF1DA1 = (Data); //Actual Data
    CANIF1DA2 = (Data);
    CANIF1DB1 = (Data);
    CANIF1DB2 = (Data);

    CANIF1CMSK |= 0x87; //Set the NEWDAT and TXRQST bit in CMSK reg

    CANIF1CRQ = objNo; //Message object number
    while(CANIF1CRQ & 0x00008000); //Wait for BUSY bit to clear

}

//This function actually receives the data put on the RX pin.
//Since registers have already been initialized in the CAN0_Receive_Set function
uint16_t CAN0_Receive_Data(uint8_t objNo)
{
    uint32_t MsgCtrl = 0;
    uint16_t data;

    CANIF2CMSK = 0x73; //ARB, CONTROL, MASK, DATAA, DATAB bits set

    CANIF2CRQ = objNo; //Message object number

    while(CANIF2CRQ & 0x00008000);//wait while CAN0 Module is BUSY

    MsgCtrl = CANIF2MCTL;

    if(MsgCtrl & 0x8000) //If NEWDAT bit is set i.e. there is new data in the data registers
    {
        data = (0x00FF & CANIF2DA1); //Get this new data.
        CANIF2CMSK |= 0x04;//Set NEWDAT in CMSK to Clear the NEWDAT bit in MCTL
    }

    if(MsgCtrl & 0x4000) //If MSGLST bit is set i.e. there was a message lost
    {
        MsgCtrl &= ~0x4000; //Clear the MSGLST bit
        CANIF2MCTL = MsgCtrl; //Clear the MSGLST bit
    }

    CANIF2CRQ = objNo; //Very important, when the message object no. is written the values in register (MCTL, ARB etc.) are updated or transfered.

    return data;
}
