#ifndef __UART_BASEAC_DFS_H__
#define __UART_BASEAC_DFS_H__


//UART Defs
#define UART_AC_BUTTON_MASK              0x20
#define UART_RECIRCULATION_BUTTON_MASK   0x04
#define UART_REAR_DEFROST_BUTTON_MASK    0x02

#define UART_AC_MODE_AUTO                0x00
#define UART_AC_MODE_FACE                0x01
#define UART_AC_MODE_FACEFEET            0x02
#define UART_AC_MODE_FEET                0x03
#define UART_AC_MODE_FEET2               0x04
#define UART_AC_MODE_FEETWIND            0x05
#define UART_AC_MODE_FEETWIND2           0x06
#define UART_AC_MODE_WINDDEFROST         0x07
    
#define UART_FAN_SPEED_OFF               0x00
#define UART_FAN_SPEED_AUTO              0x01

#define UART_A1_RESP_AC_ON               0x02
#define UART_A1_RESP_RECIR_ON            0x08
#define UART_A1_RESP_RECIR_OFF           0x04
#define UART_A1_RESP_RDEF_ON             0x40

//{Toggle flag states} = 0, Mode, Off, WindDef, Auto, RearDef, Recirc, AC
        //{Mode} = 0-7
        //{Fan} = 0=Off, 1=Auto, 2-26=FanSpeeds
        //{Temp} = 0-30
        //{prev flag states} = 0,0,0,windDefON,AutoON,prevRearDef,prevRecirc,prevAC


#define DIAL_CTRL_STATE_TOGGLE_INDEX     0
#define DIAL_CTRL_STATE_MODE_INDEX       1
#define DIAL_CTRL_STATE_FAN_INDEX        2
#define DIAL_CTRL_STATE_TEMP_INDEX       3
#define DIAL_CTRL_STATE_PREV_INDEX       4

#define DIAL_CTRL_STATE_AC_MASK          0x01
#define DIAL_CTRL_STATE_RECIRC_MASK      0x02
#define DIAL_CTRL_STATE_RDEF_MASK        0x04
#define DIAL_CTRL_STATE_AUTO_MASK        0x08
#define DIAL_CTRL_STATE_WDEF_MASK        0x10
#define DIAL_CTRL_STATE_OFF_MASK         0x20
#define DIAL_CTRL_STATE_MODE_MASK        0x40

//CAN Defs
#define CAN_AC_MODE_AUTO                 0x00
#define CAN_AC_MODE_FACE                 0x00
#define CAN_AC_MODE_FACEFEET             0x00
#define CAN_AC_MODE_FEET                 0x00
#define CAN_AC_MODE_FEETWIND             0x00

//Operatinal parameters
#define TEMP_F //Comment out for Dec C
#ifdef TEMP_F
#define TEMP_OFFSET                      60 //degF
#else            
#define TEMP_OFFSET                      20  //degC
#endif

#endif