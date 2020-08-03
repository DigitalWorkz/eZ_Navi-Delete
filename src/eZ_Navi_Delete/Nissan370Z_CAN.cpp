#include <SPI.h>
#include <mcp2515.h>
#include "Nissan370Z_CAN.h"


Nissan370Z_CAN::Nissan370Z_CAN(MCP2515* can)
{
    _canBus = can;
    _isInitialized = false;
    _roll = 0;
}

Nissan370Z_CAN::~Nissan370Z_CAN()
{
}

bool Nissan370Z_CAN::begin()
{
    //CAN Filtering
    _canBus->reset();
    if(_canBus->setBitrate(CAN_500KBPS) == MCP2515::ERROR_OK)
    {
        //set filters to only buffer AC messages
        _canBus->setFilterMask(MCP2515::MASK0,0,0x0000054E); //0x54A | 0x54C
        _canBus->setFilterMask(MCP2515::MASK1,0,0x0000076F); //0x54B | 0x625
        _canBus->setFilter(MCP2515::RXF0,0,0x0000054A);
        _canBus->setFilter(MCP2515::RXF1,0,0x0000054C);
        _canBus->setFilter(MCP2515::RXF2,0,0x0000054B);
        _canBus->setFilter(MCP2515::RXF3,0,0x00000625);
        
        _canBus->setNormalMode();
        _isInitialized = true;
    }
    return _isInitialized;
}

void Nissan370Z_CAN::ISR_can_rx()
{
    uint8_t interupts = _canBus->getInterrupts();
    ///*if(interupts & ~0x03)*/ Serial.println(interupts, BIN);
    //check for errors? Clear and return if error?
    //if((bool)(interupts & 0b10100000))
    //{
    //    Serial.println(_canBus->getErrorFlags(), BIN);
    //    _canBus->clearMERR();
    //    _canBus->clearERRIF();
    //    //_canBus->clearInterrupts();
    //    return;
    //}
    while((bool)(interupts & 0b00000011))
    {
        struct can_frame canMsg;
        if(_canBus->readMessage(&canMsg) == MCP2515::ERROR_OK)
        {
            switch(canMsg.can_id)
            {
                case 0x54A:
                {
                    for(uint8_t i = 0; i < canMsg.can_dlc; ++i) _can_54A_data_v.data[i] = canMsg.data[i];
                    _can_counters_v[0]++;
                    break;
                }
                case 0x54B:
                {
                    for(uint8_t i = 0; i < canMsg.can_dlc; ++i) _can_54B_data_v.data[i] = canMsg.data[i];
                    _can_counters_v[1]++;
                    break;
                }
                case 0x54C:
                {
                    for(uint8_t i = 0; i < canMsg.can_dlc; ++i) _can_54C_data_v.data[i] = canMsg.data[i];
                    _can_counters_v[2]++;
                    break;
                }
                case 0x625:
                {
                    for(uint8_t i = 0; i < canMsg.can_dlc; ++i) _can_625_data_v.data[i] = canMsg.data[i];
                    _can_counters_v[3]++;
                    break;
                }
                default:{}
            }
        }
        interupts = _canBus->getInterrupts();
    }
    _canBus->clearInterrupts();
}

uint8_t* Nissan370Z_CAN::get54AData()
{
    noInterrupts();
    //write transfer buffer from volatile buffer and return _can_54A_data;
    for(uint8_t i = 0; i < _can_54A_data_v.can_dlc; ++i) _can_54A_data.data[i] = _can_54A_data_v.data[i];
    interrupts();
    return _can_54A_data.data;
}

uint8_t* Nissan370Z_CAN::get54BData()
{
    noInterrupts();
    //write transfer buffer from volatile buffer and return _can_54A_data;
    for(uint8_t i = 0; i < _can_54B_data_v.can_dlc; ++i) _can_54B_data.data[i] = _can_54B_data_v.data[i];
    interrupts();
    return _can_54B_data.data;
}

uint8_t* Nissan370Z_CAN::get54CData()
{
    noInterrupts();
    //write transfer buffer from volatile buffer and return _can_54A_data;
    for(uint8_t i = 0; i < _can_54C_data_v.can_dlc; ++i) _can_54C_data.data[i] = _can_54C_data_v.data[i];
    interrupts();
    return _can_54C_data.data;
}

uint8_t* Nissan370Z_CAN::get625Data()
{
    noInterrupts();
    //write transfer buffer from volatile buffer and return _can_54A_data;
    for(uint8_t i = 0; i < _can_625_data_v.can_dlc; ++i) _can_625_data.data[i] = _can_625_data_v.data[i];
    interrupts();
    return _can_625_data.data;
}

uint8_t* Nissan370Z_CAN::get540Data()
{
    return _can_540_state.data;
}

uint8_t* Nissan370Z_CAN::get541Data()
{
    return _can_541_state.data;
}

uint8_t* Nissan370Z_CAN::get542Data()
{
    return _can_542_state.data;
}

uint8_t* Nissan370Z_CAN::getDBG()
{
    //assign counter values and return pointer to this data buffer
    _can_DBG_state.data[3] = _can_counters_v[0];
    _can_DBG_state.data[4] = _can_counters_v[1];
    _can_DBG_state.data[5] = _can_counters_v[2];
    _can_DBG_state.data[6] = _can_counters_v[3];
    return _can_DBG_state.data;
}

void Nissan370Z_CAN::broadcastCAN()
{
    _can_540_state.data[7] = _roll;
    _can_541_state.data[7] = (uint8_t)0x80|_roll;
    _can_542_state.data[7] = _roll;


///FIX ME!: Need to scope with CAN traffic and debug toggles
//    noInterrupts();
    _canBus->sendMessage(&_can_540_state);
//    interrupts();
//    noInterrupts();
    _canBus->sendMessage(&_can_541_state);
//    interrupts();
    // reset rear heating flag after sending ... this is a special case.
    _can_541_state.data[1] &= ~0x80;  //_can2.writeFlag(1, B10000000, B00000000);
//    noInterrupts();
    _canBus->sendMessage(&_can_542_state);
//    interrupts();
    if (_roll < 3) _roll++;
    else _roll = 0;
}

void Nissan370Z_CAN::broadcastDEBUG()
{
//    noInterrupts();
    _canBus->sendMessage(&_can_DBG_state);
//    interrupts();
}