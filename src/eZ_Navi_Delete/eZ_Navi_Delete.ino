#include "Nissan370Z_AC_System.h"
#include <HardwareSerial.h>
#include <mcp2515.h>


HardwareSerial Serial2(PA3, PA2); //needed for STM32 HAL Core

MCP2515* mcp_can1;
Nissan370Z_AC_System* AC_System;

void setup()
{
    pinMode(BT_MODE_PIN, INPUT);
    if(!digitalRead(BT_MODE_PIN)) uartPassThrough(); //This never returns

    pinMode(CAN_INT_PIN, INPUT);
    mcp_can1 = new MCP2515(CAN_CS_PIN);
    AC_System = new Nissan370Z_AC_System(&EXT_COMS_SERIAL, mcp_can1);
    //AC_System = new Nissan370Z_AC_System(&EXT_COMS_SERIAL, mcp_can1);
    AC_System->begin();
    attachInterrupt(CAN_INT_PIN, CAN_ISR, LOW); //LOW only available in STM32 HAL Core
    //attachInterrupt(CAN_INT_PIN, CAN_ISR, FALLING);
}

void CAN_ISR()
{
    AC_System->can_isr();
}

void loop()
{
    AC_System->checkDialCom();
    AC_System->sendDialResponse();
    AC_System->updateCAN();

    ///Fix me!:
    //Periodically check the INT pin isnt stuck due to falling edge during SPI transfer
    //This shouldnt be needed, but sometimes a falling is missed which results in a
    //situation where the ISR never gets called again. This ensures it will but is not
    //desired operation.
    //if(!digitalRead(CAN_INT_PIN)) AC_System->can_isr();
}

void uartPassThrough()
{
    Serial.begin(115200);
    Serial1.begin(115200);
    while(!Serial);
    
    //Run UART passthough until power cycle
    while(true)
    {
        if(Serial.available()) Serial1.write(Serial.read());
        if(Serial1.available()) Serial.write(Serial1.read());
    }
}
