#include "Nissan370Z_AC_System.h"
#include "uart_baseAC_dfs.h"
#include "niscan_dfs.h"

Nissan370Z_AC_System::Nissan370Z_AC_System(HardwareSerial* uart, MCP2515* can)
{    
    _dial = new Nissan370Z_AC_Dial(uart);
    _can = new Nissan370Z_CAN(can);
    _sendA0 = true;
}

Nissan370Z_AC_System::~Nissan370Z_AC_System()
{
    delete _dial;
    delete _can;
}

void Nissan370Z_AC_System::can_isr()
{
    _can->ISR_can_rx();
}

void Nissan370Z_AC_System::begin()
{
    _dial->begin();
    if(!_can->begin())
    {
        //error
        while(1);
    }
}

void Nissan370Z_AC_System::syncSystem()
{
    //Update CAN messages with Dial State
    updateCanData(_dial->getDialState(), _can->get541Data(), _can->get542Data());
}

void Nissan370Z_AC_System::updateCanData(uint8_t* dialState, uint8_t* data541, uint8_t* data542) //can2
{
    uint8_t curr_CAN_mode = (_can->get54BData())[NIS_AC2_MODE_IDX];
    uint8_t temperature = dialState[DIAL_CTRL_STATE_TEMP_INDEX];
    uint8_t fanSpeed = dialState[DIAL_CTRL_STATE_FAN_INDEX];

    if((bool)(dialState[DIAL_CTRL_STATE_TOGGLE_INDEX] & DIAL_CTRL_STATE_OFF_MASK))       data541[2] ^= 0x01; //_can2.toggleFlag(2, B00000001); -> Toggle OFF button
    //_can2.writeFlag(1, B10000000, B10000000); //always write to 1 ... rear heating is different from all other flags
    if((bool)(dialState[DIAL_CTRL_STATE_TOGGLE_INDEX] & DIAL_CTRL_STATE_RDEF_MASK))      data541[1] |= 0x80; //Toggle Rear Def Button
    if(fanSpeed == UART_FAN_SPEED_OFF)
    {
        _dial->clearDialStateToggles();
        return;
    }

    if((bool)(dialState[DIAL_CTRL_STATE_TOGGLE_INDEX] & DIAL_CTRL_STATE_AUTO_MASK))      data541[3] ^= 0x80; //_can2.toggleFlag(3, B10000000); -> Toggle AUTO button
    
    #ifdef TEMP_F
    if (temperature >= 0x3C && temperature <= 0x5A) data542[1] = temperature;//_can3.writeByte(1, temperature);
    #else //TEMP_C
    if (temperature >= 0x20 && temperature <= 0x3C) data542[1] = temperature;//_can3.writeByte(1, temperature);
    #endif
    data541[2] ^= 0x80; //1 << 7; //_can2.toggleFlag(2, B10000000); -> temp

    if(dialState[DIAL_CTRL_STATE_MODE_INDEX] == UART_AC_MODE_AUTO)
    {
        _dial->clearDialStateToggles();
        return;
    }

    if((bool)(dialState[DIAL_CTRL_STATE_TOGGLE_INDEX] & DIAL_CTRL_STATE_AC_MASK))        data541[2] ^= 0x04; //_can2.toggleFlag(2, B00000100); -> Toggle AC button
    if((bool)(dialState[DIAL_CTRL_STATE_TOGGLE_INDEX] & DIAL_CTRL_STATE_RECIRC_MASK))    data541[1] ^= 0x02; //_can2.toggleFlag(1, B00000010); -> Toggle RECIRC button
    if((bool)(dialState[DIAL_CTRL_STATE_TOGGLE_INDEX] & DIAL_CTRL_STATE_WDEF_MASK))      data541[0] ^= 0x01; //_can2.toggleFlag(0, B00000001); -> Toggle WDEF button

    everyl(500) //double the CAN message rate to ensure non-stale data compare
    {
        if(//dialState[DIAL_CTRL_STATE_MODE_INDEX] != UART_AC_MODE_AUTO &&
           dialState[DIAL_CTRL_STATE_MODE_INDEX] != UART_AC_MODE_WINDDEFROST &&
           //dialState[DIAL_CTRL_STATE_FAN_INDEX] != UART_FAN_SPEED_OFF &&
           convertToCANMode(dialState[DIAL_CTRL_STATE_MODE_INDEX]) != curr_CAN_mode)
           {
               data541[0] ^= 0x10; //_can2.toggleFlag(0, B00010000);
           }
    }

    //always toggle temp and fan flags to always update the byte data - because, why not?
    if (fanSpeed >= 0 && fanSpeed <= 7) data542[0] = fanSpeed * 8; //_can3.writeByte(0, fanSpeed * 8);
    data541[3] ^= 0x10; //_can2.toggleFlag(3, B00010000); -> fan
    
    _dial->clearDialStateToggles();
}

uint8_t Nissan370Z_AC_System::convertToCANMode(uint8_t dialMode)
{
    switch(dialMode)
    {
        case UART_FAN_SPEED_OFF:
        {
            return NIS_AC2_MODE_OFF;
        }
        case UART_AC_MODE_FACE:
        {
            return NIS_AC2_MODE_FACE;
        }
        case UART_AC_MODE_FACEFEET:
        {
            return NIS_AC2_MODE_FACEFEET;
        }
        case UART_AC_MODE_FEET:
        case UART_AC_MODE_FEET2:
        {
            return NIS_AC2_MODE_FEET;
        }
        case UART_AC_MODE_FEETWIND:
        case UART_AC_MODE_FEETWIND2:
        {
            return NIS_AC2_MODE_WINDFEET;
        }
    }
    return 0;
}

void Nissan370Z_AC_System::sendDialResponse()
{
    everyl(20)
    {    
        if(_sendA0)
        {
            byte a0[] = {0xA0, 0x00, 0x00, 0x00, 0x00, 0x00};
            _dial->sendDialResponse(a0, sizeof(a0));
            _sendA0 = false;
        }
        else
        {
            byte a1[] = {0xA1, 0x00, 0x00, 0x00, 0x00, 0x00};
            a1[2] = a1[5] = syncA1Response();
            _dial->sendDialResponse(a1, sizeof(a1));
            _sendA0 = true;
        }
    }
}

void Nissan370Z_AC_System::checkDialCom()
{
    _dial->checkDialCom();
}

uint8_t Nissan370Z_AC_System::syncA1Response()
{
    uint8_t a1Response = 0;
    //get can data
    uint8_t* can54B = _can->get54BData();
    uint8_t* can54C = _can->get54CData();
    uint8_t* can625 = _can->get625Data();
    
    //define response based on masking
    if((bool)(can54B[NIS_AC2_RECI_IDX] & NIS_AC2_RECI_ON)) a1Response |= UART_A1_RESP_RECIR_ON;
    if((bool)(can54B[NIS_AC2_RECI_IDX] & NIS_AC2_RECI_OFF)) a1Response |= UART_A1_RESP_RECIR_OFF;
    if((bool)(can54C[NIS_AC3_AC_IDX] & NIS_AC3_AC_ON)) a1Response |= UART_A1_RESP_AC_ON;
    if((bool)(can625[NIS_AC4_RH_IDX] & NIS_AC4_RH_ON)) a1Response |= UART_A1_RESP_RDEF_ON;
    return a1Response;
}

void Nissan370Z_AC_System::updateCAN()
{
    everyl(100)
    {
        syncSystem();
        _can->broadcastCAN();
    }
    everyl(150)
    {
        setCANDebug();
        _can->broadcastDEBUG();
    }
}

//Debug
void Nissan370Z_AC_System::setCANDebug()
{
    uint8_t* dial = _dial->getDialState();
    //uint8_t* can54A = _can->get54AData();
    //uint8_t* can54B = _can->get54BData();
    //uint8_t* can54C = _can->get54CData();
    //uint8_t* can625 = _can->get625Data();
    
    ///byte dialData[8] = {ccSystem->getDialCtrlMode(), ccSystem->getDialCtrlFan(), ccSystem->getDialCtrlTemp(), AC1_cnt, AC2_cnt, AC3_cnt, AC4_cnt, 0x00};
    uint8_t* debug = _can->getDBG();
    debug[0] = dial[DIAL_CTRL_STATE_MODE_INDEX];
    debug[1] = dial[DIAL_CTRL_STATE_FAN_INDEX];
    debug[2] = dial[DIAL_CTRL_STATE_TEMP_INDEX];
    //debug[3] = can54A[CAN_DATA_BUFF_LEN - 1]; //counter
    //debug[4] = can54B[CAN_DATA_BUFF_LEN - 1]; //counter
    //debug[5] = can54C[CAN_DATA_BUFF_LEN - 1]; //counter
    //debug[6] = can625[CAN_DATA_BUFF_LEN - 1]; //counter
    //debug[7] = 0;
    //debug[3] = dial[DIAL_CTRL_STATE_TOGGLE_INDEX];
}