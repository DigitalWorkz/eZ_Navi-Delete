#ifndef __EZ_NAVIDELETE_BLD_CNF_H__
#define __EZ_NAVIDELETE_BLD_CNF_H__

/**************************************************************************
    User Selectable Options
**************************************************************************/
/// >>> Uncomment desired interface (ONLY ONE!) <<<
//#define __USE_EZ_BLUETOOTH_COMS
//#define __USE_EZ_USB_COMS
#define __USE_EZ_DIAL_COMS

/// >>> Comment out to use deg C <<<
#define TEMP_F 

/**************************************************************************
**************************************************************************/


//Debug
//#define USE_STM32_CORE



#ifdef __USE_EZ_BLUETOOTH_COMS
//#define __HAS_BLUETOOTH
#define EXT_COMS_SERIAL eZ_BT

#elif defined __USE_EZ_DIAL_COMS
#define EXT_COMS_SERIAL eZ_DIAL

#else //__USE_EZ_DIAL_COMS
#define EXT_COMS_SERIAL USB_DIAL
#endif

#ifdef TEMP_F
#define TEMP_VALID      temperature >= 0x3C && temperature <= 0x5A
#define TEMP_OFFSET     60//degF
#define UNIT_DEF        0x28
#else
#define TEMP_VALID      temperature >= 0x20 && temperature <= 0x3C
#define TEMP_OFFSET     32//degC
#define UNIT_DEF        0x20
#endif

#endif
