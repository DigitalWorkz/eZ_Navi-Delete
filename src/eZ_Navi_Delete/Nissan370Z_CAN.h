#ifndef __Nissan370Z_CAN_H__
#define __Nissan370Z_CAN_H__
#include <mcp2515.h>

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

    struct can_frame _can_540_state = {0x540, 8, {0x28,0x64,0,0,0,0,0,0}};
    struct can_frame _can_541_state = {0x541, 8, {0,0,0,0,0,0,0,0}};
    struct can_frame _can_542_state = {0x542, 8, {0,0,0,0,0,0,0,0}};
    struct can_frame _can_DBG_state = {0x777, 8, {0,0,0,0,0,0,0,0}};

    bool _isInitialized;
    uint8_t _roll;



    public:
    Nissan370Z_CAN(MCP2515*);
    ~Nissan370Z_CAN();
    bool begin();
    void ISR_can_rx();
    uint8_t* get54AData();
    uint8_t* get54BData();
    uint8_t* get54CData();
    uint8_t* get625Data();
    uint8_t* get540Data();
    uint8_t* get541Data();
    uint8_t* get542Data();
    uint8_t* getDBG();
    void broadcastCAN();
    void broadcastDEBUG();
};

#endif