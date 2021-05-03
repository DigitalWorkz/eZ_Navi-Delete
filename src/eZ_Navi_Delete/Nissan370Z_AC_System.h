#ifndef __Nissan370Z_AC_System__
#define __Nissan370Z_AC_System__
#include "Nissan370Z_AC_Dial.h"
#include "Nissan370Z_CAN.h"
#include "src\everytime\src\everytime.h"
#include <Arduino.h>

/*
//
// FOR STM32 ONLY
// 
// PinMap:
// CAN_SPI_CS       PA4(14)
// CAN_SPI_CLK      PA5(15)
// CAN_SPI_MISO     PA6(16)
// CAN_SPI_MOSI     PA7(17)
// CAN_SPI_INT      PB0(18)
// SW_A             PA0(10)
// SW_B             PA1(11)
// DIAL_UART2_Tx    PA2(12)
// DIAL_UART2_Rx    PA3(13)
// BT_UART_Tx       PA9(30)
// BT_UART_Rx       PA10(31)
// USB_D+           PA11(32)
// USB_D-           PA12(33)
// I2C2_SCL2        PB10(21)
// I2C2_SDA2        PB11(22)
*/

#define CAN_INT_PIN PB0
#define CAN_CS_PIN PA4

class Nissan370Z_AC_System
{
    Nissan370Z_AC_Dial* _dial;
    Nissan370Z_CAN* _can;
    bool _sendA0;

    //Update the A1 status responce based on the CAN state
    uint8_t syncA1Response();
    void syncSystem();
    //Update the CAN control messages based on the dial state
    void updateCanData(uint8_t*, uint8_t*, uint8_t*);
    //Convert the UART modes to CAN modes
    uint8_t convertToCANMode(uint8_t);

    public:
    Nissan370Z_AC_System(HardwareSerial*, MCP2515*);
    ~Nissan370Z_AC_System();
    //ISR wrapper
    void can_isr();
    //Init the dial and CAN interfaces
    void begin();
    //Wrapper to call dial function
    void checkDialCom();
    //Send the A0 and A1 responces
    void sendDialResponse();
    //Scheduled CAN sync and braodcast of control commands
    void updateCAN();
    //Scheduled CAN braodcast for debug
    void setCANDebug();
};

#endif