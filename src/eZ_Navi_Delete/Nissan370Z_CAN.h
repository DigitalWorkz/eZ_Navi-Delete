#ifndef __Nissan370Z_CAN_H__
#define __Nissan370Z_CAN_H__

#include "eZ_NaviDelete_Build_Conf.h"
#include <mcp2515.h>

//NISSAN 370Z CAN CONTROL DEFINITIONS
//0x54A
#define NIS_AC1_IDX             0x54A
#define NIS_AC1_TEMP_IDX        4
#define NIS_AC1_TEMP_MSK        B11111111

//0x54B
#define NIS_AC2_IDX             0x54B
#define NIS_AC2_AUTO_IDX        1
#define NIS_AC2_AUTO_MSK        B00001110
#define NIS_AC2_AUTO_ON         B00000110
#define NIS_AC2_AUTO_OFF        B00001000

#define NIS_AC2_MODE_IDX        2
#define NIS_AC2_MODE_OFF        0x80
#define NIS_AC2_MODE_FEET       0x98
#define NIS_AC2_MODE_WINDFEET   0xA0
#define NIS_AC2_MODE_FACE       0x88
#define NIS_AC2_MODE_FACEFEET   0x90

#define NIS_AC2_WIND_IDX        3
#define NIS_AC2_WIND_MSK        B10000000
#define NIS_AC2_WIND_ON         B10000000
#define NIS_AC2_WIND_OFF        B00000000

#define NIS_AC2_RECI_IDX        3
#define NIS_AC2_RECI_MSK        B00000011
#define NIS_AC2_RECI_ON         B00000001
#define NIS_AC2_RECI_OFF        B00000010

#define NIS_AC2_FANS_IDX        4
#define NIS_AC2_FANS_OFF        04
#define NIS_AC2_FANS_1          0x0C
#define NIS_AC2_FANS_2          0x14
#define NIS_AC2_FANS_3          0x1C
#define NIS_AC2_FANS_4          0x24
#define NIS_AC2_FANS_5          0x2C
#define NIS_AC2_FANS_6          0x34
#define NIS_AC2_FANS_7          0x3C

//0x54C
#define NIS_AC3_IDX             0x54C
#define NIS_AC3_AC_IDX          2
#define NIS_AC3_AC_MSK          B10000000
#define NIS_AC3_AC_ON           B10000000
#define NIS_AC3_AC_OFF          B00000000

//0x625
#define NIS_AC4_IDX             0x625
#define NIS_AC4_RH_IDX          0
#define NIS_AC4_RH_MSK          B00000001
#define NIS_AC4_RH_ON           B00000001

class Nissan370Z_CAN
{
    private:
    MCP2515* _canBus;
    
    volatile struct can_frame _can_54A_data_v = {0x54A, 8, {0,0,0,0,0,0,0,0}};
    volatile struct can_frame _can_54B_data_v = {0x54B, 8, {0,0,0,0,0,0,0,0}};
    volatile struct can_frame _can_54C_data_v = {0x54C, 8, {0,0,0,0,0,0,0,0}};
    volatile struct can_frame _can_625_data_v = {0x625, 8, {0,0,0,0,0,0,0,0}};
    volatile uint8_t _can_counters_v[4] = {0,0,0,0};

    struct can_frame _can_54A_data = {0x54A, 8, {0,0,0,0,0,0,0,0}};
    struct can_frame _can_54B_data = {0x54B, 8, {0,0,0,0,0,0,0,0}};
    struct can_frame _can_54C_data = {0x54C, 8, {0,0,0,0,0,0,0,0}};
    struct can_frame _can_625_data = {0x625, 8, {0,0,0,0,0,0,0,0}};
    uint8_t _can_counters[4] = {0,0,0,0};

    struct can_frame _can_540_state = {0x540, 8, {UNIT_DEF,0x64,0,0,0,0,0,0}};
    struct can_frame _can_541_state = {0x541, 8, {0,0,0,0,0,0,0,0}};
    struct can_frame _can_542_state = {0x542, 8, {0,0,0,0,0,0,0,0}};
    struct can_frame _can_DBG_state = {0x777, 8, {0,0,0,0,0,0,0,0}};

    bool _isInitialized;
    uint8_t _roll;

    public:
    Nissan370Z_CAN(MCP2515*);
    ~Nissan370Z_CAN();
    //Setup CAN Bus
    bool begin();
    //ISR to service CAN Controller
    void ISR_can_rx();
    //Return a pointer to the array containing the 0x54A data received
    uint8_t* get54AData();
    //Return a pointer to the array containing the 0x54B data received
    uint8_t* get54BData();
    //Return a pointer to the array containing the 0x54C data received
    uint8_t* get54CData();
    //Return a pointer to the array containing the 0x625 data received
    uint8_t* get625Data();
    //Return a pointer to the array containing the 0x540 data to send
    uint8_t* get540Data();
    //Return a pointer to the array containing the 0x541 data to send
    uint8_t* get541Data();
    //Return a pointer to the array containing the 0x542 data to send
    uint8_t* get542Data();
    //Return a pointer to the array containing the debug data to send
    uint8_t* getDBG();
    //Transmit CAN messages (AC controll messages)
    void broadcastCAN();
    //Transmit CAN messages (debug status message)
    void broadcastDEBUG();
};

#endif