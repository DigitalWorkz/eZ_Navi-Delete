#include "Nissan370Z_AC_System.h"
#include <mcp2515.h>

#ifdef USE_STM32_CORE
#include <HardwareSerial.h>
HardwareSerial eZ_DIAL(PA3, PA2); //needed for STM32 HAL Core
#endif

MCP2515* mcp_can1;

#ifdef __USE_EZ_BLUETOOTH_COMS
    Nissan370Z_BT_AC_System* AC_System;
#elif defined __USE_EZ_DIAL_COMS
    //Nissan370Z_Dial_AC_System* AC_System;
    Nissan370Z_AC_System* AC_System;
#else
    Nissan370Z_USB_AC_System* AC_System;
#endif


void setup()
{
    pinMode(CAN_INT_PIN, INPUT);
    mcp_can1 = new MCP2515(CAN_CS_PIN);
    
    #ifdef __USE_EZ_BLUETOOTH_COMS
    pinMode(BT_MODE_PIN, INPUT);
    delay(10);
    if(!digitalRead(BT_MODE_PIN)) uartPassThrough(); //This never returns
    
    AC_System = new Nissan370Z_BT_AC_System(&eZ_BT, mcp_can1);    
    #elif defined __USE_EZ_DIAL_COMS
    //AC_System = new Nissan370Z_Dial_AC_System(&eZ_DIAL, mcp_can1);;
    AC_System = new Nissan370Z_AC_System(&eZ_DIAL, mcp_can1);;
    #else
    AC_System = new Nissan370Z_USB_AC_System(&eZ_USB, mcp_can1);;
    #endif
    
    //AC_System = new Nissan370Z_AC_System(&EXT_COMS_SERIAL, mcp_can1);
    AC_System->begin();
    
    #ifdef USE_STM32_CORE
    attachInterrupt(CAN_INT_PIN, CAN_ISR, LOW); //LOW only available in STM32 HAL Core
    #else
    attachInterrupt(CAN_INT_PIN, CAN_ISR, FALLING);
    #endif
}

void CAN_ISR()
{
    AC_System->can_isr();
}

void loop()
{
    AC_System->run();
}

#ifdef __USE_EZ_BLUETOOTH_COMS
void uartPassThrough()
{
    eZ_USB.begin(115200);
    eZ_BT.begin(115200);
    while(!Serial);
    
    //Run UART passthough until power cycle
    while(true)
    {
        if(eZ_USB.available()) eZ_BT.write(eZ_USB.read());
        if(eZ_BT.available()) eZ_USB.write(eZ_BT.read());
    }
}
#endif
