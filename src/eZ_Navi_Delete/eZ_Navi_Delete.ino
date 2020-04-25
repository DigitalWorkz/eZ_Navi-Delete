
#include "Nissan370Z_AC_System.h"
#include <HardwareSerial.h>
#include <mcp2515.h>
//#include <Arduino.h>

MCP2515* mcp_can1;
Nissan370Z_AC_System* AC_System;

void setup()
{
    pinMode(CAN_INT_PIN, INPUT);
    mcp_can1 = new MCP2515(CAN_CS_PIN);
    AC_System = new Nissan370Z_AC_System(&Serial2, mcp_can1);
    AC_System->begin();
    attachInterrupt(CAN_INT_PIN, CAN_ISR, FALLING);
}

void CAN_ISR()
{
    //do{
       AC_System->can_isr();
    //}while(!digitalRead(CAN_INT_PIN));
}

void loop()
{
    AC_System->checkDialCom();
    AC_System->sendDialResponse();
    AC_System->updateCAN();
    //periodically check the INT pin isnt stuck due to falling edge during SPI transfer
    if(!digitalRead(CAN_INT_PIN)) AC_System->can_isr();
}
