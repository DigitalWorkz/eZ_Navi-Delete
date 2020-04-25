#ifndef __Nissan370Z_AC_Dial_H__
#define __Nissan370Z_AC_Dial_H__
#include <HardwareSerial.h>


#define PREV_B0_AC_REC  _b0_data[1]

#define PREV_B1_MODE    _b1_data[1]
#define PREV_B1_FAN     _b1_data[2]
#define PREV_B1_RDEF    _b1_data[3]

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
    void _writeStateData(uint8_t*, uint8_t);
    void _updateStatus();
    void _updateDialData(uint8_t*);
    void _updateB0DialStatus(uint8_t*);
    void _updateB1DialStatus(uint8_t*);
    
    public:
    Nissan370Z_AC_Dial(HardwareSerial*);
    ~Nissan370Z_AC_Dial();
    void begin();
    void checkDialCom();   //Check UART buffer for complete B0 or B1 transmission
    void sendDialResponse(uint8_t*, uint8_t);
    uint8_t* getB0Data();  //retrun pointer to B0 data
    uint8_t* getB1Data();  //return pointer to B1 data
    uint8_t* getDialState();
    void clearDialStateToggles();
};

#endif