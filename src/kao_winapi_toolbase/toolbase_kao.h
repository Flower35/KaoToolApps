/**************************************************************/
/* "kao_winapi_toolbase/toolbase_kao.h"                       */
/**************************************************************/

#ifndef H_KAO_WINAPI_TOOLBASE_KAO
#define H_KAO_WINAPI_TOOLBASE_KAO

#include "toolbase_main.h"

/**
 * @defgroup ToolBase_Kao ToolBase :: Kao
 * @{
 */


/**************************************************************/
/* Kao :: Constants                                           */
/**************************************************************/

/**
 * @def ENUMERATED_PROCESSES
 * Number of PIDs (Process Identifiers) to enumerate with WinApi calls.
 * @def KAO2PL_WND_CLASS
 * Class name registered for "kangurek Kao: 2ga runda" window
 * ("GLUT" from "OpenGL Utility Toolkit" library).
 * @def KAO2PL_WND_NAME
 * Window name of the retail version of "Kao the Kangaroo: Round 2".
 * @def KAO2PL_EXEC_PREFIX
 * The prefix of "Kao the Kangaroo: Round 2" executable. Normal filename
 * is "kao2.exe", but there are suffixes added for modded versions of the game.
 * @def KAO3PL_WND_CLASS
 * Class name registered for "Kangurek Kao: Tajemnica Wulkanu" window
 * ("GLUT" from "OpenGL Utility Toolkit" library).
 * @def KAO3PL_WND_NAME
 * Window name of the retail version of "Kao: Mystery of the Volcano".
 * @def KAO3PL_EXEC_PREFIX
 * The prefix of "Kao: Mystery of the Volcano" executable. Normal filename
 * is "kao_tw.exe", but there are suffixes added for modded versions of the game.
 * @def KAO_GAMEVERSION_2PL
 * Identifies selected game version ("Kao the Kangaroo: Round 2").
 * @def KAO_GAMEVERSION_3PL
 * Identifies selected game version ("Kao: Mystery of the Volcano").
 */

#define ENUMERATED_PROCESSES  512

#define KAO2PL_WND_CLASS    "GLUT"
#define KAO2PL_WND_NAME     "kangurek Kao: 2ga runda"
#define KAO2PL_EXEC_PREFIX  "kao2"

#define KAO3PL_WND_CLASS    "GLUT"
#define KAO3PL_WND_NAME     "Kangurek Kao: Tajemnica Wulkanu"
#define KAO3PL_EXEC_PREFIX  "kao_tw"

#define KAO_GAMEVERSION_2PL  1
#define KAO_GAMEVERSION_3PL  2


/**************************************************************/
/* Kao :: Helper macros                                       */
/**************************************************************/

/**
 * @def KAO_GET_BYTE(__addr)
 * Macro for reading one byte from given game memory address
 * into local variable `byteTest`.
 * @def KAO_PUT_BYTE(__addr, __value)
 * Macro for storing one byte at given game memory address,
 * by setting local variable `byteTest` to the specified value.
 * @def KAO_WAIT_BYTE_EQ(__addr, __value)
 * Macro that stops code execution of this ToolApp, as long as one byte
 * from fiven game memory address is still equal to the specified value.
 * @def KAO_WAIT_BYTE_NE(__addr, __value)
 * Macro that stops code execution of this ToolApp, as long as one byte
 * from fiven game memory address is still different from the specified value.
 * @def KAO_GET_WORD(__addr)
 * Macro for reading two bytes from given game memory address
 * into local variable `wordTest`.
 * @def KAO_PUT_WORD(__addr, __value)
 * Macro for storing two bytes at given game memory address,
 * by setting local variable `wordTest` to the specified value.
 * @def KAO_WAIT_WORD_EQ(__addr, __value)
 * Macro that stops code execution of this ToolApp, as long as two bytes
 * from fiven game memory address is still equal to the specified value.
 * @def KAO_WAIT_WORD_NE(__addr, __value)
 * Macro that stops code execution of this ToolApp, as long as two bytes
 * from fiven game memory address is still different from the specified value.
 * @def KAO_GET_DWORD(__addr)
 * Macro for reading four bytes from given game memory address
 * into local variable `dwordTest`.
 * @def KAO_PUT_DWORD(__addr, __value)
 * Macro for storing four bytes at given game memory address,
 * by setting local variable `dwordTest` to the specified value.
 * @def KAO_INC_DWORD(__addr)
 * Macro for increasing by one the four-byte integer
 * stored at given game-memory address.
 * @def KAO_WAIT_DWORD_EQ(__addr, __value)
 * Macro that stops code execution of this ToolApp, as long as four bytes
 * from fiven game memory address is still equal to the specified value.
 * @def KAO_WAIT_DWORD_NE(__addr, __value)
 * Macro that stops code execution of this ToolApp, as long as four bytes
 * from fiven game memory address is still different from the specified value.
 */

#define KAO_GET_BYTE(__addr) \
  TRY_BOOL(Kao_readMem(__addr, &(byteTest), 0x01))

#define KAO_PUT_BYTE(__addr, __value) \
  byteTest = __value; \
  TRY_BOOL(Kao_writeMem(__addr, &(byteTest), 0x01))

#define KAO_WAIT_BYTE_EQ(__addr, __value) \
  do { \
    TRY_BOOL(Kao_readMem(__addr, &(byteTest), 0x01)) \
  } while (__value == byteTest);

#define KAO_WAIT_BYTE_NE(__addr, __value) \
  do { \
    TRY_BOOL(Kao_readMem(__addr, &(byteTest), 0x01)) \
  } while (__value != byteTest);

#define KAO_GET_WORD(__addr) \
  TRY_BOOL(Kao_readMem(__addr, &(wordTest), 0x02))

#define KAO_PUT_WORD(__addr, __value) \
  wordTest = __value; \
  TRY_BOOL(Kao_writeMem(__addr, &(wordTest), 0x02))

#define KAO_WAIT_WORD_EQ(__addr, __value) \
  do { \
    TRY_BOOL(Kao_readMem(__addr, &(wordTest), 0x02)) \
  } while (__value == wordTest);

#define KAO_WAIT_WORD_NE(__addr, __value) \
  do { \
    TRY_BOOL(Kao_readMem(__addr, &(wordTest), 0x02)) \
  } while (__value != wordTest);

#define KAO_GET_DWORD(__addr) \
  TRY_BOOL(Kao_readMem(__addr, &(dwordTest), 0x04))

#define KAO_PUT_DWORD(__addr, __value) \
  dwordTest = __value; \
  TRY_BOOL(Kao_writeMem(__addr, &(dwordTest), 0x04))

#define KAO_INC_DWORD(__addr) \
  KAO_GET_DWORD(__addr) \
  KAO_PUT_DWORD(__addr, (dwordTest + 1))

#define KAO_WAIT_DWORD_EQ(__addr, __value) \
  do { \
    TRY_BOOL(Kao_readMem(__addr, &(dwordTest), 0x04)) \
  } while (__value == dwordTest);

#define KAO_WAIT_DWORD_NE(__addr, __value) \
  do { \
    TRY_BOOL(Kao_readMem(__addr, &(dwordTest), 0x04)) \
  } while (__value != dwordTest);


/**************************************************************/
/* Kao :: Public data                                         */
/**************************************************************/

/**
 * @brief Handle to the active game window.
 */
extern HWND g_Kao_gameWindow;

/**
 * @brief Handle to the active game process.
 */
extern HANDLE g_Kao_gameHandle;

/**
 * @brief Identifier of the currenly selected game version.
 */
extern int g_Kao_gameVersion;


/**************************************************************/
/* Kao :: Public functions                                    */
/**************************************************************/

/**
 * @brief Writes data to the game process.
 *
 * @param address The 32-bit pointer in game memory space.
 * @param from Data to be transfered.
 * @param length Number of bytes to transfer.
 * @return TRUE if writing was successful, FALSE otherwise.
 * @note Sets global error message on failure.
 */
BOOL Kao_writeMem(DWORD address, LPCVOID from, size_t length);

/**
 * @brief Reads data from the game process.
 *
 * @param address The 32-bit pointer in game memory space.
 * @param into Data to be replaced.
 * @param length Number of bytes to transfer.
 * @return TRUE if reading was successful, FALSE otherwise.
 * @note Sets global error message on failure.
 */
BOOL Kao_readMem(DWORD address, LPVOID into, size_t length);

/**
 * @brief Initializing global data.
 *
 * @return TRUE if initialization was successful, FALSE otherwise.
 */
BOOL Kao_init();

/**
 * @brief Try to attach game process and set related global pointers.
 *
 * @returns TRUE if the game is ready for hacking, FALSE otherwise.
 */
BOOL Kao_startSession(int version);

/**
 * @brief Invalidate game handle and related global pointers.
 *
 * @param checkGameHandle Check and close the handle to the game process.
 */
VOID Kao_invalidateSession(BOOL checkGameHandle);

/**
 * @brief Checks game state in the tool-app loop.
 *
 * Updates specific flags if the game process is attached.
 *
 * @note If the game was closed, `g_Kao_gameHandle` will be set
 * to `INVALID_HANDLE_VALUE`.
 *
 * @return TRUE if the game is still running and not in a loading state,
 * FALSE otherwise.
 */
BOOL Kao_checkGameState();

/**
 * @brief Closing global game process handle.
 */
VOID Kao_end();


/**************************************************************/
/* Kao :: Public functions that require definitions           */
/**************************************************************/

/**
 * @brief Called once, when the game is attached for the first time.
 *
 * @note This function's body must be defined in a specific tool-app.
 */
VOID Kao_gameAttachedEx();

/**
 * @brief Called once, when the game is closed.
 *
 * @note This function's body must be defined in a specific tool-app.
 */
VOID Kao_gameDetachedEx();

/**
 * @brief Called once, when the game starts loading a new level.
 *
 * @note This function's body must be defined in a specific tool-app.
 */
VOID Kao_gameStartedLoadingEx();

/**
 * @brief Called once, when the game finishes loading a new level.
 *
 * @note This function's body must be defined in a specific tool-app.
 */
VOID Kao_gameFinishedLoadingEx();

/**
 * @brief Initializing tool-app pivate global data.
 *
 * Called from `Kao_init()`.
 *
 * @note This function's body must be defined in a specific tool-app.
 *
 * @return TRUE if initialization was successful, FALSE otherwise.
 */
BOOL Kao_initEx();

/**
 * @brief Cleaning tool-app private global data.
 *
 * Called from `Kao_end()`.
 *
 * @note This function's body must be defined in a specific tool-app.
 */
VOID Kao_endEx();


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO_WINAPI_TOOLBASE_KAO */


/**************************************************************/
