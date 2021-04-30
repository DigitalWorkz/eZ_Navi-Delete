
#include "Nissan370Z_AC_System.h"
#include <HardwareSerial.h>
#include <mcp2515.h>

MCP2515* mcp_can1;
Nissan370Z_AC_System* AC_System;

void setup()
{
    //disable unused ports
    pinMode(PA8, INPUT);  //BT Config
    pinMode(PA9, INPUT);  //UART1 Tx
    pinMode(PA10, INPUT); //UART1 Rx
    pinMode(PB10, INPUT); //SCL2
    pinMode(PB11, INPUT); //SDA2
    pinMode(PB15, OUTPUT_OPEN_DRAIN); //BT Reset
    digitalWrite(PB15, LOW); //hold BT in reset
    
    pinMode(CAN_INT_PIN, INPUT);
    mcp_can1 = new MCP2515(CAN_CS_PIN);
    AC_System = new Nissan370Z_AC_System(&Serial2, mcp_can1);
    AC_System->begin();
    attachInterrupt(CAN_INT_PIN, CAN_ISR, FALLING);
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
    if(!digitalRead(CAN_INT_PIN)) AC_System->can_isr();
}
