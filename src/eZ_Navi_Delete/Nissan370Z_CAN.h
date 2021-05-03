#ifndef __Nissan370Z_CAN_H__
#define __Nissan370Z_CAN_H__
#include "mcp2515.h"

#define CAN_DATA_BUFF_LEN 9
#define CAN_STAT_BUFF_LEN 8

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

    struct can_frame _can_540_state = {0x540, 8, {0x28,0x64,0,0,0,0,0,0}}; //0x28 and 0x64 = US model?
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
