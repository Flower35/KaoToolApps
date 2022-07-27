/**************************************************************/
/* "kao_winapi_toolbase/toolbase_kao_hacks.h"                 */
/**************************************************************/

#ifndef H_KAO_WINAPI_TOOLBASE_KAO_HACKS
#define H_KAO_WINAPI_TOOLBASE_KAO_HACKS

#include "toolbase_main.h"

/**
 * @defgroup ToolBase_Kao_Hacks ToolBase :: Kao Hacks
 * @{
 */


/**************************************************************/
/*  Kao Hacks :: Constants                                     */
/**************************************************************/

/**
 * @def KAOHACK_COMM_SKIP
 * "ToolApp <-> Game" communication: The game skips checking
 * for any queued calls and runs as intended.
 * @def KAOHACK_COMM_WAITING
 * "ToolApp <-> Game" communication: The game is stuck in an infinite loop
 * until "awaken" by the ToolApp itself.
 * @def KAOHACK_COMM_CDECL
 * "ToolApp <-> Game" communication: x86asm function must be called
 * using the "cdecl" convention.
 * @def KAOHACK_COMM_STDCALL
 * "ToolApp <-> Game" communication: x86asm function must be called
 * using the "stdcall" convention.
 * @def KAOHACK_COMM_THISCALL
 * "ToolApp <-> Game" communication: x86asm function must be called
 * using the "thiscall" convention.
 * @def KAO2PL_COMMHACK01_ADDR
 * Destination address for the "Communicator Hack 1"
 * in "Kao the Kangaroo: Round 2".
 * @def KAO2PL_COMMHACK01_SIZE
 * Size (in bytes) of the `KAO2PL_COMMHACK01_ADDR` array.
 * @def KAO2PL_COMMHACK02_ADDR
 * Destination address for the "Communicator Hack 2"
 * in "Kao the Kangaroo: Round 2".
 * @def KAO2PL_COMMHACK02_SIZE
 * Size (in bytes) of the `KAO2PL_COMMHACK02_SIZE` array.
 * @def KAO3PL_COMMHACK01_ADDR
 * Destination address for the "Communicator Hack 1"
 * in "Kao: Mystery of the Volcano".
 * @def KAO3PL_COMMHACK01_SIZE
 * Size (in bytes) of the `KAO3PL_COMMHACK01_ADDR` array.
 * @def KAO3PL_COMMHACK02_ADDR
 * Destination address for the "Communicator Hack 2"
 * in "Kao: Mystery of the Volcano"
 * @def KAO3PL_COMMHACK02_SIZE
 * Size (in bytes) of the `KAO3PL_COMMHACK02_ADDR` array.
 */

#define KAOHACK_COMM_SKIP      0
#define KAOHACK_COMM_WAITING   1
#define KAOHACK_COMM_CDECL     2
#define KAOHACK_COMM_STDCALL   3
#define KAOHACK_COMM_THISCALL  4

#define KAO2PL_COMMHACK01_ADDR  0x004902D6
#define KAO2PL_COMMHACK01_SIZE  18

#define KAO2PL_COMMHACK02_ADDR  0x005CC660
#define KAO2PL_COMMHACK02_SIZE  117

#define KAO3PL_COMMHACK01_ADDR  0x00465D06
#define KAO3PL_COMMHACK01_SIZE  18

#define KAO3PL_COMMHACK02_ADDR  0x00696F10
#define KAO3PL_COMMHACK02_SIZE  117


/**************************************************************/
/* Kao Hacks :: Helper macros                                 */
/**************************************************************/

/**
 * @def TRY_PUT_KAOHACK
 * Tries to write selected game-hack into the running game instance.
 */

#define TRY_PUT_KAOHACK(__name) \
  TRY_BOOL(Kao_writeMem(__name##_ADDR, __name##_CODE, __name##_SIZE))


/**************************************************************/
/*  Kao Hacks :: Public data                                  */
/**************************************************************/

/**
 * @brief Actual code (x86asm instructions) for the "Communicator Hack 1"
 * in "Kao the Kangaroo: Round 2".
 */
extern uint8_t KAO2PL_COMMHACK01_CODE[KAO2PL_COMMHACK01_SIZE];

/**
 * @brief Actual code (x86asm instructions) for the "Communicator Hack 2"
 * in "Kao the Kangaroo: Round 2".
 */
extern uint8_t KAO2PL_COMMHACK02_CODE[KAO2PL_COMMHACK02_SIZE];

/**
 * @brief Actual code (x86asm instructions) for the "Communicator Hack 1"
 * in "Kao: Mystery of the Volcano".
 */
extern uint8_t KAO3PL_COMMHACK01_CODE[KAO3PL_COMMHACK01_SIZE];

/**
 * @brief Actual code (x86asm instructions) for the "Communicator Hack 2"
 * in "Kao: Mystery of the Volcano".
 */
extern uint8_t KAO3PL_COMMHACK02_CODE[KAO3PL_COMMHACK02_SIZE];


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO_WINAPI_TOOLBASE_KAO_HACKS */


/**************************************************************/
