#ifndef __Nissan370Z_AC_Dial_H__
#define __Nissan370Z_AC_Dial_H__
#include <HardwareSerial.h>

//UART Dial Data Definitions
#define UART_AC_BUTTON_MASK             0x20
#define UART_RECIRCULATION_BUTTON_MASK  0x04
#define UART_REAR_DEFROST_BUTTON_MASK   0x02

#define UART_AC_MODE_AUTO               0x00
#define UART_AC_MODE_FACE               0x01
#define UART_AC_MODE_FACEFEET           0x02
#define UART_AC_MODE_FEET               0x03
#define UART_AC_MODE_FEET2              0x04
#define UART_AC_MODE_FEETWIND           0x05
#define UART_AC_MODE_FEETWIND2          0x06
#define UART_AC_MODE_WINDDEFROST        0x07

#define UART_FAN_SPEED_OFF              0x00
#define UART_FAN_SPEED_AUTO             0x01

#define UART_A1_RESP_AC_ON              0x02
#define UART_A1_RESP_RECIR_ON           0x08
#define UART_A1_RESP_RECIR_OFF          0x04
#define UART_A1_RESP_RDEF_ON            0x40

//Dial Status Indices
#define DIAL_CTRL_STATE_TOGGLE_INDEX    0
#define DIAL_CTRL_STATE_MODE_INDEX      1
#define DIAL_CTRL_STATE_FAN_INDEX       2
#define DIAL_CTRL_STATE_TEMP_INDEX      3
#define DIAL_CTRL_STATE_PREV_INDEX      4

//Dial Status Masks
#define DIAL_CTRL_STATE_AC_MASK         0x01
#define DIAL_CTRL_STATE_RECIRC_MASK     0x02
#define DIAL_CTRL_STATE_RDEF_MASK       0x04
#define DIAL_CTRL_STATE_AUTO_MASK       0x08
#define DIAL_CTRL_STATE_WDEF_MASK       0x10
#define DIAL_CTRL_STATE_OFF_MASK        0x20
#define DIAL_CTRL_STATE_MODE_MASK       0x40

//Aliases
#define PREV_B0_AC_REC                  _b0_data[1]
#define PREV_B1_MODE                    _b1_data[1]
#define PREV_B1_FAN                     _b1_data[2]
#define PREV_B1_RDEF                    _b1_data[3]

class Nissan370Z_AC_Dial
{
    HardwareSerial* _dial;

    //raw data from dial
    uint8_t _b0_data[5] = {0,0,0,0,0};
    uint8_t _b1_data[5] = {0,0xFF,0,0,0};

    //status to update CAN including toggle requests
    //{Toggle flag states} = 0, Mode, Off, WindDef, Auto, RearDef, Recirc, AC
    //{Mode} = 0-7
    //{Fan} = 0=Off, 1=Auto, 2-26=FanSpeeds
    //{Temp} = 0-30
    //{prev flag states} = 0,0,0,windDefON,AutoON,prevRearDef,prevRecirc,prevAC  <-- UNUSED CURRENTLY DUE TO STORING B0 and B1 RAW DATA
    uint8_t _dial_status[5] = {0,0,0,0,0};

    private:
    //Determine the dial data type 
    void _updateDialData(uint8_t*);
    //Update the B0 relavent fields in the _dial_status array
    void _updateB0DialStatus(uint8_t*);
    //Update the B1 relavent fields in the _dial_status array
    void _updateB1DialStatus(uint8_t*);

    public:
    Nissan370Z_AC_Dial(HardwareSerial*);
    ~Nissan370Z_AC_Dial();
    void begin();
    //Check UART buffer for complete B0 or B1 transmission
    void checkDialCom();
    //Send A0 or A1 response
    void sendDialResponse(uint8_t*, uint8_t);
    //retrun pointer to B0 data
    uint8_t* getB0Data();
    //return pointer to B1 data
    uint8_t* getB1Data();
    //Return a pointer to the _dial_status data array
    uint8_t* getDialState();
    //Clear all the toggle requests
    void clearDialStateToggles();
};

#endif