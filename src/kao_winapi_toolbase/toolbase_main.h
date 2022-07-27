/**************************************************************/
/* "kao_winapi_toolbase/toolbase_main.h"                      */
/**************************************************************/

#ifndef H_KAO_WINAPI_TOOLBASE_MAIN
#define H_KAO_WINAPI_TOOLBASE_MAIN

/**
 * @defgroup ToolBase_Main ToolBase :: Main
 * @{
 */


/**************************************************************/
/* Target platform guard                                      */
/**************************************************************/

#if !defined(_WIN32)
  #error("Client target platform is Win32! :)")
#endif


/**************************************************************/
/* Unicode support                                            */
/**************************************************************/

#define UNICODE
#define _UNICODE

#include <tchar.h>


/**************************************************************/
/* C Libraries                                                */
/**************************************************************/

#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <psapi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**************************************************************/
/* ToolBase :: Common definitions                             */
/**************************************************************/

#include "../kao_tools_common/common.h"


/**************************************************************/
/* ToolBase :: Misc definitions                               */
/**************************************************************/

/**
 * @def BUF_SIZE
 * Length (in characters) of a buffer for building default size
 * messages with `snprintf()`.
 * @def MEDIUM_BUF_SIZE
 * Length (in characters) of a buffer for building medium-sized
 * messages with `snprintf()`.
 * @def SMALL_BUF_SIZE
 * Length (in characters) of a buffer for building small-sized
 * messages with `snprintf()`.
 * @def LARGE_BUF_SIZE
 * Length (in characters) of a buffer for building very large
 * messages with `snprintf()`.
 * @def LASTMSG_SIZE
 * Length (in characters) of a buffer containing ToolApp status messages.
 */

#define BUF_SIZE          256
#define MEDIUM_BUF_SIZE    64
#define SMALL_BUF_SIZE     32
#define LARGE_BUF_SIZE   1024
#define LASTMSG_SIZE    (BUF_SIZE / 2)


/**************************************************************/
/* ToolBase :: Helper macros                                  */
/**************************************************************/

/**
 * @def IS_INVALID(__a)
 * Tests if the handle `__a` is invalid.
 * @def NOT_INVALID(__a)
 * Tests if the handle `__a` is valid.
 */

#define  IS_INVALID(__a)  EQ(INVALID_HANDLE_VALUE, __a)
#define NOT_INVALID(__a)  NE(INVALID_HANDLE_VALUE, __a)


/**************************************************************/
/* ToolBase :: Public data                                    */
/**************************************************************/

/**
 * @brief Flag that indicates whether the application should be closed.
 */
extern BOOL g_quitTest;

/**
 * @brief Buffer that stores characters for last error message.
 */
extern TCHAR g_lastMessage[LASTMSG_SIZE];

/**
 * @brief Global flags that control the flow of the tool-app.
 */
extern DWORD g_controlFlags;


/**************************************************************/
/* ToolBase :: Public functions                               */
/**************************************************************/

/**
 * @brief Function that shall be called from `main()`.
 *
 * Prepares all the modules and enters the application loop.
 * @param hInstance Handle to the current application instance.
 * @param windowTitle Defines the main window title.
 * @param windowWidth Defines main window width in pixels.
 * @param windowHeight Defines main window height in pixels.
 * @return Status code (return value) of the program.
 */
INT ToolBase_run(
  HINSTANCE hInstance,
  LPCTSTR windowTitle,
  DWORD windowWidth,
  DWORD windowHeight);


/**************************************************************/
/* ToolBase :: Public functions that require definitions      */
/**************************************************************/

/**
 * @brief Single iteration of the tool-app loop.
 *
 * Called after processing Windows messages and checking the game state.
 * @note This function's body must be defined in a specific tool-app.
 *
 * @param isGameReady TRUE if the game is attached and in a valid state.
 * @return TRUE if the application is still running, FALSE otherwise.
 */
BOOL ToolBase_loopEx(BOOL isGameReady);


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO_WINAPI_TOOLBASE_MAIN */


/**************************************************************/
