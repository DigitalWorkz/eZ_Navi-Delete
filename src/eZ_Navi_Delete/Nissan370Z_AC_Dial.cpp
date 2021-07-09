#include "Nissan370Z_AC_Dial.h"
#include "uart_baseAC_dfs.h"
#include <Arduino.h>

Nissan370Z_AC_Dial::Nissan370Z_AC_Dial(HardwareSerial* DialUart)
{
    _dial = DialUart;
}

Nissan370Z_AC_Dial::~Nissan370Z_AC_Dial()
{
}

void Nissan370Z_AC_Dial::begin()
{
    _dial->begin(9600, SERIAL_8O1);
}

void Nissan370Z_AC_Dial::checkDialCom()
{    
    //read full CMD from serial buffer
    if(_dial->available() < 5) return;

    uint8_t cmd[5];
    
    uint8_t cmdType = _dial->read();
    if(cmdType != 0xB0 && cmdType != 0xB1) return;
    
    
    uint8_t xorResult = 0;
    uint8_t cmdCheck[3];
    for(uint8_t i = 0; i < sizeof(cmdCheck); ++i)
    {
        cmdCheck[i] = _dial->read();
        if(i == 0) xorResult = cmdCheck[0];
        else xorResult ^= cmdCheck[i];
    }

    if(xorResult != _dial->read()) return;
    cmd[0] = cmdType;
    cmd[1] = cmdCheck[0];
    cmd[2] = cmdCheck[1];
    cmd[3] = cmdCheck[2];
    cmd[4] = xorResult;
    
    //valid cmd received
    _updateDialData(cmd);
}

void Nissan370Z_AC_Dial::_updateDialData(uint8_t* cmd)
{
    switch (cmd[0])
    {
        case 0xB0:
        {
            _updateB0DialStatus(cmd);
            memcpy(_b0_data, cmd, sizeof(_b0_data));
            break;
        }
        case 0xB1:
        {
            _updateB1DialStatus(cmd);
            memcpy(_b1_data, cmd, sizeof(_b1_data));
            break;
        }
    }
}

void Nissan370Z_AC_Dial::_updateB0DialStatus(uint8_t* cmd)
{
    uint8_t acRec = cmd[1];
    uint8_t temp = cmd[3];
    
    bool boolResult = (bool)(acRec & UART_RECIRCULATION_BUTTON_MASK);
    //code to convert B0 data into CAN values and store
    if (boolResult != (bool)(PREV_B0_AC_REC & UART_RECIRCULATION_BUTTON_MASK))
    {
        if (boolResult) _dial_status[DIAL_CTRL_STATE_TOGGLE_INDEX] |= DIAL_CTRL_STATE_RECIRC_MASK;
    }

    if(_dial_status[DIAL_CTRL_STATE_FAN_INDEX] == UART_FAN_SPEED_OFF) return;
    
    boolResult = (bool)(acRec & UART_AC_BUTTON_MASK);
    if (boolResult != (bool)(PREV_B0_AC_REC & UART_AC_BUTTON_MASK))
    { 
        if (boolResult) _dial_status[DIAL_CTRL_STATE_TOGGLE_INDEX] |= DIAL_CTRL_STATE_AC_MASK;
    }

    _dial_status[DIAL_CTRL_STATE_TEMP_INDEX] = (temp + TEMP_OFFSET);
}

void Nissan370Z_AC_Dial::_updateB1DialStatus(uint8_t* cmd)
{

    uint8_t modeByte = cmd[1];
    uint8_t fanByte = cmd[2];
    uint8_t rearDefByte = cmd[3];

    bool boolResult = (bool)(rearDefByte & UART_REAR_DEFROST_BUTTON_MASK);
    if (boolResult != (bool)(PREV_B1_RDEF & UART_REAR_DEFROST_BUTTON_MASK))
    {
        if (boolResult) _dial_status[DIAL_CTRL_STATE_TOGGLE_INDEX] |= DIAL_CTRL_STATE_RDEF_MASK;
    }

    switch (fanByte)
    {
        case UART_FAN_SPEED_OFF: //0
        {
            if(PREV_B1_FAN != UART_FAN_SPEED_OFF)
            {
                _dial_status[DIAL_CTRL_STATE_TOGGLE_INDEX] |= DIAL_CTRL_STATE_OFF_MASK;
                _dial_status[DIAL_CTRL_STATE_FAN_INDEX] = 0;
                return;
            }
            return;
        }
        case UART_FAN_SPEED_AUTO: //1
        {
            //???
            //cmd[2] = UART_FAN_SPEED_OFF;
            return;
        }
        default: //map fan speed
        {
            if (fanByte < 6)   {_dial_status[DIAL_CTRL_STATE_FAN_INDEX] = 1; break;} //2-5
            if (fanByte < 10)  {_dial_status[DIAL_CTRL_STATE_FAN_INDEX] = 2; break;} //6-9
            if (fanByte < 14)  {_dial_status[DIAL_CTRL_STATE_FAN_INDEX] = 3; break;} //10-13
            if (fanByte < 18)  {_dial_status[DIAL_CTRL_STATE_FAN_INDEX] = 4; break;} //14-17
            if (fanByte < 22)  {_dial_status[DIAL_CTRL_STATE_FAN_INDEX] = 5; break;} //18-21
            if (fanByte < 26)  {_dial_status[DIAL_CTRL_STATE_FAN_INDEX] = 6; break;} //22-25
            if (fanByte == 26) {_dial_status[DIAL_CTRL_STATE_FAN_INDEX] = 7; break;} //26
        }
    }

    //update Mode
    _dial_status[DIAL_CTRL_STATE_MODE_INDEX] = modeByte;

    //Set mode toggles (Auto and Windshield Defrost)
    switch (modeByte)
    {
        case UART_AC_MODE_AUTO: //0, Auto mode is a button press - request toggle if present
        {
            //if the previous Auto state is false
            if(PREV_B1_FAN == UART_FAN_SPEED_OFF || PREV_B1_FAN == UART_FAN_SPEED_AUTO || PREV_B1_MODE != UART_AC_MODE_AUTO)//toggle auto if OFF, if ON - ignore
            {
                _dial_status[DIAL_CTRL_STATE_TOGGLE_INDEX] |= DIAL_CTRL_STATE_AUTO_MASK;//set auto state ON
            }
            break;
        }
        case UART_AC_MODE_WINDDEFROST: //7
        {
            if(PREV_B1_MODE != UART_AC_MODE_WINDDEFROST) //toggle defrost if OFF, if On - ignore
            {
                _dial_status[DIAL_CTRL_STATE_TOGGLE_INDEX] |= DIAL_CTRL_STATE_WDEF_MASK; //set defrost state ON
            }
            break;
        }
        default: {}
    }
}

void Nissan370Z_AC_Dial::sendDialResponse(uint8_t* data, uint8_t len)
{    
    _dial->write(data, len);
}

uint8_t* Nissan370Z_AC_Dial::getB0Data()
{
    return _b0_data;
}

uint8_t* Nissan370Z_AC_Dial::getB1Data()
{
    return _b1_data;
}

uint8_t* Nissan370Z_AC_Dial::getDialState()
{
    return _dial_status;
}

void Nissan370Z_AC_Dial::clearDialStateToggles()
{
    _dial_status[DIAL_CTRL_STATE_TOGGLE_INDEX] = 0x00;
}
