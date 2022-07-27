/**************************************************************/
/* "kao2_xray/xray_main.c"                                    */
/**************************************************************/

#include "xray_main.h"

#include "xray_gui.h"
#include "xray_nodes.h"

#include "../kao_winapi_toolbase/toolbase_kao.h"
#include "../kao_winapi_toolbase/toolbase_kao_mem.h"
#include "../kao_winapi_toolbase/toolbase_kao_hacks.h"
#include "../kao_winapi_toolbase/toolbase_anyarray.h"

/**
 * @addtogroup KaoToolApp_XRay_Main
 * @{
 */


/**************************************************************/
/* Kao2 XRay Tool :: Special public functions                 */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
ToolBase_loopEx(BOOL isGameReady)
{
  BOOL test;
  DWORD controlFlag;
  BOOL (*toolAction)(BOOL) = NULL;

  /* Attach Kao 2 */
  if EQ(XRAY_ACT_ATTACH_KAO2, g_controlFlags)
  {
    Kao_startSession(KAO_GAMEVERSION_2PL);

    g_controlFlags = XRAY_ACT_NONE;
  }
  /* Attach Kao 3 */
  else if EQ(XRAY_ACT_ATTACH_KAO3, g_controlFlags)
  {
    Kao_startSession(KAO_GAMEVERSION_3PL);

    g_controlFlags = XRAY_ACT_NONE;
  }
  else if NOT_ZERO(g_controlFlags)
  {
    test = HIWORD(g_controlFlags);
    controlFlag = LOWORD(g_controlFlags);

    if IS_FALSE(isGameReady)
    {
      g_lastMessage[0] = '\0';
      GUI_displayError(TEXT("The game is busy or not attached yet!"));
    }
    /* Toggle Invisible Collision */
    else if EQ(XRAY_ACT_TOGGLE_INVIS, controlFlag)
    {
      toolAction = &(KaoXRayNodes_toggleInvisibleCollision);
    }
    /* Toggle Box Triggers */
    else if EQ(XRAY_ACT_TOGGLE_TRIGG, controlFlag)
    {
      toolAction = &(KaoXRayNodes_toggleBoxZones);
    }

    if (NULL != toolAction)
    {
      GUI_updateStatus(TEXT("Please wait... (switch focus to the game window!)"));

      test = toolAction(test);
      KaoMem_commAwake();

      if IS_FALSE(test)
      {
        GUI_displayError(TEXT("Error while performing memory operations!"));
      }
    }

    g_controlFlags = XRAY_ACT_NONE;
  }

  return TRUE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
Kao_gameAttachedEx()
{
  KaoXRayNodes_clear();
  KaoXRayGUI_clear();
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
Kao_gameDetachedEx()
{
  KaoXRayNodes_clear();
  KaoXRayGUI_clear();
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
Kao_gameStartedLoadingEx()
{
  DWORD dwordTest;
  DWORD gamelet;
  DWORD levelId;
  WCHAR buf[LARGE_BUF_SIZE];
  WCHAR levelName[MEDIUM_BUF_SIZE];

  /********************************/

  KaoXRayNodes_clear();
  KaoXRayGUI_clear();

  /********************************/

  KAO_GET_DWORD(g_KaoOffsets->GLOBAL_GAMELET);
  gamelet = dwordTest;

  KAO_GET_DWORD(gamelet + g_KaoOffsets->GAMELET_FLOWCTRL)
  if IS_ZERO(dwordTest)
  {
    goto leave;
  }
  levelId = dwordTest - 1;

  KAO_GET_DWORD(g_KaoOffsets->GLOBAL_ARRAY_REF_LEVELDESC + 0x04);
  if (levelId >= dwordTest)
  {
    goto leave;
  }

  TRY_BOOL(KaoMem_readLevelName(levelName, levelId))

  _snwprintf(buf, LARGE_BUF_SIZE, L"Loading \"%s\"...", levelName);
  GUI_updateStatus(buf);
  return;

  leave:
  {
    GUI_updateStatus(TEXT("Loading some level..."));
  }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
Kao_gameFinishedLoadingEx()
{
  DWORD dwordTest;
  WCHAR buf[LARGE_BUF_SIZE];
  WCHAR levelName[MEDIUM_BUF_SIZE];

  /********************************/

  KaoXRayGUI_clear();

  /********************************/

  KAO_GET_DWORD(g_KaoOffsets->GLOBAL_LEVELID);
  TRY_BOOL(KaoMem_readLevelName(levelName, dwordTest))

  _snwprintf(buf, LARGE_BUF_SIZE, L"Level \"%s\" loaded.", levelName);
  GUI_updateStatus(buf);
  return;

  leave:
  {
    GUI_updateStatus(TEXT("Level loaded."));
  }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
Kao_initEx()
{
  KaoXRayNodes_init();

  return TRUE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
Kao_endEx()
{
  KaoXRayNodes_clear();
}


/**************************************************************/
/* Program entry point                                        */
/**************************************************************/

INT WINAPI
WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR pCmdLine,
  int nCmdShow)
{
  return ToolBase_run(
    hInstance,
    TEXT(GUI_WND_LABEL),
    GUI_WND_WIDTH,
    GUI_WND_HEIGHT);
}


/**************************************************************/

/**
 * @}
 */


/**************************************************************/
