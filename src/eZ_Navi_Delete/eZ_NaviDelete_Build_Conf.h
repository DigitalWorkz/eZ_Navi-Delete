#ifndef __EZ_NAVIDELETE_BLD_CNF_H__
#define __EZ_NAVIDELETE_BLD_CNF_H__

//#define ENABLE_BT_PASSTHROUGH

#define EXT_COMS_SERIAL eZ_DIAL
//#define EXT_COMS_SERIAL eZ_USB
//#define EXT_COMS_SERIAL eZ_BT

//Operatinal parameters
#define TEMP_F //Comment out for Dec C

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
