/**************************************************************/
/* "kao_winapi_toolbase/toolbase_kao_hacks.c"                 */
/**************************************************************/

#include "toolbase_kao_hacks.h"

#include "toolbase_kao_mem.h"

#include "toolbase_kao.h"

/**
 * @addtogroup ToolBase_Kao_Hacks
 * @{
 */


/**************************************************************/
/* Kao Hacks :: Public data                                   */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint8_t KAO2PL_COMMHACK01_CODE[KAO2PL_COMMHACK01_SIZE] =
{
  0xE8,0x85,0xC3,0x13,0x00,0x83,0x3D,0xDC,0x67,0x62,0x00,0x00,0x74,0x28,0x90,0x90,0x90,0x90
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint8_t KAO2PL_COMMHACK02_CODE[KAO2PL_COMMHACK02_SIZE] =
{
  0x55,0x89,0xE5,0x53,0x56,0x57,0xA1,0x00,0x1E,0x64,0x00,0x89,0xC1,0x83,0xE0,0x07,0x83,0xF8,0x00,0x74,0x5B,0xC1,0xE9,0x03,0x89,0xCB,0xC1,0xE3,0x02,0x31,0xD2,0x31,0xF6,0x83,0xF8,0x02,0x74,0x13,0x83,0xF8,0x03,0x74,0x0E,0x83,0xF8,0x04,0x75,0x35,0x8B,0x15,0x08,0x1E,0x64,0x00,0x83,0xC6,0x04,0x81,0xC6,0x08,0x1E,0x64,0x00,0x29,0xDC,0x89,0xE7,0x83,0xF8,0x02,0x74,0x02,0x31,0xDB,0xF3,0xA5,0x89,0xD1,0xFF,0x15,0x04,0x1E,0x64,0x00,0x01,0xDC,0xA3,0x04,0x1E,0x64,0x00,0xC7,0x05,0x00,0x1E,0x64,0x00,0x01,0x00,0x00,0x00,0x83,0x3D,0x00,0x1E,0x64,0x00,0x01,0x74,0xF7,0xEB,0x96,0x5F,0x5E,0x5B,0x5D,0xC3
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint8_t KAO3PL_COMMHACK01_CODE[KAO3PL_COMMHACK01_SIZE] =
{
  0xE8,0x05,0x12,0x23,0x00,0x83,0x3D,0x18,0xC9,0x75,0x00,0x00,0x74,0x28,0x90,0x90,0x90,0x90
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint8_t KAO3PL_COMMHACK02_CODE[KAO3PL_COMMHACK02_SIZE] =
{
  0x55,0x89,0xE5,0x53,0x56,0x57,0xA1,0x40,0x50,0x78,0x00,0x89,0xC1,0x83,0xE0,0x07,0x83,0xF8,0x00,0x74,0x5B,0xC1,0xE9,0x03,0x89,0xCB,0xC1,0xE3,0x02,0x31,0xD2,0x31,0xF6,0x83,0xF8,0x02,0x74,0x13,0x83,0xF8,0x03,0x74,0x0E,0x83,0xF8,0x04,0x75,0x35,0x8B,0x15,0x48,0x50,0x78,0x00,0x83,0xC6,0x04,0x81,0xC6,0x48,0x50,0x78,0x00,0x29,0xDC,0x89,0xE7,0x83,0xF8,0x02,0x74,0x02,0x31,0xDB,0xF3,0xA5,0x89,0xD1,0xFF,0x15,0x44,0x50,0x78,0x00,0x01,0xDC,0xA3,0x44,0x50,0x78,0x00,0xC7,0x05,0x40,0x50,0x78,0x00,0x01,0x00,0x00,0x00,0x83,0x3D,0x40,0x50,0x78,0x00,0x01,0x74,0xF7,0xEB,0x96,0x5F,0x5E,0x5B,0x5D,0xC3
};


/**************************************************************/

/**
 * @}
 */


/**************************************************************/
