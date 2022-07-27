/**************************************************************/
/* "kao_winapi_toolbase/toolbase_main.c"                      */
/**************************************************************/

#include "toolbase_main.h"

#include "toolbase_kao_mem.h"
#include "toolbase_kao.h"
#include "toolbase_gui.h"

/**
 * @addtogroup ToolBase_Main
 * @{
 */


/**************************************************************/
/* ToolBase :: Public data                                    */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL g_quitTest;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
TCHAR g_lastMessage[LASTMSG_SIZE];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
DWORD g_controlFlags;


/**************************************************************/
/* ToolBase: Private functions                                */
/**************************************************************/

/**
 * @brief Single iteration of the tool-app loop.
 *
 * Processes window messages and tool-app logic.
 * @return TRUE if the application is still running, FALSE otherwise.
 */
BOOL
ToolBase_loop()
{
  if IS_FALSE(GUI_windowLoop())
  {
    return FALSE;
  }

  return ToolBase_loopEx(
    Kao_checkGameState());
}


/**************************************************************/
/* ToolBase: Public functions                                 */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
INT
ToolBase_run(
  HINSTANCE hInstance,
  LPCTSTR windowTitle,
  DWORD windowWidth,
  DWORD windowHeight)
{
  INT status_code = EXIT_FAILURE;
  BOOL gui_initialized = FALSE;
  BOOL kao_initialized = FALSE;

  /* Reset external data */

  g_quitTest = FALSE;

  g_controlFlags = 0;

  /* Initialize other modules */

  if IS_FALSE(
    GUI_init(
      hInstance,
      windowTitle,
      windowWidth,
      windowHeight))
  {
    goto leave;
  }
  else
  {
    gui_initialized = TRUE;
  }

  if IS_FALSE(Kao_init())
  {
    goto leave;
  }
  else
  {
    kao_initialized = TRUE;
  }

  KaoMem_init();

  /* Starting the program */

  while (ToolBase_loop())
  {}

  /* Leaving the program */

  status_code = EXIT_SUCCESS;

  leave:
  {
    if (kao_initialized)
    {
      Kao_end();
    }

    if (gui_initialized)
    {
      GUI_end();
    }
  }

  return status_code;
}


/**************************************************************/

/**
 * @}
 */


/**************************************************************/
