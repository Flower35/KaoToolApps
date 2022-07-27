/**************************************************************/
/* "kao_winapi_toolbase/toolbase_kao.c"                       */
/**************************************************************/

#include "toolbase_kao.h"
#include "toolbase_kao_mem.h"
#include "toolbase_kao_hacks.h"

#include "toolbase_gui.h"

/**
 * @addtogroup ToolBase_Kao
 * @{
 */


/**************************************************************/
/* Kao :: Private data                                        */
/**************************************************************/

/**
 * @brief Flag that remembers last Loading State.
 */
BOOLEAN g_Kao_lastLoadingState;


/**************************************************************/
/* Kao :: Public data                                         */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
HWND g_Kao_gameWindow;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
HANDLE g_Kao_gameHandle;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int g_Kao_gameVersion;


/**************************************************************/
/* Kao :: Private functions                                   */
/**************************************************************/

/**
 * @brief Matches a window with process ID
 *
 * @param found_window Handle to a window that is a perfect candidate
 * for the game window.
 * @param found_pid Handle to any valid Windows process.
 * @return TRUE if `found_window` belongs to `found_pid` process.
 */
BOOL
Kao_matchGameWindowWithProcess(
  const HWND found_window,
  const DWORD found_pid)
{
  DWORD pid = 0;
  GetWindowThreadProcessId(found_window, &(pid));

  return EQ(found_pid, pid);
}

/**
 * @brief Looks for the game process and tries to attach it.
 *
 * This function searches for the game executable in Windows processes list.
 * Then it tries to attach the process for reading and writing operations.
 * Game window's class name and title are validated.
 * @param class_name Game window's class (usually "GLUT").
 * @param window_name Defines window title for specific game version.
 * @param exec_prefix Only acceptable prefix for a game executable.
 * @note On success: `INVALID_HANDLE_VALUE != g_Kao_gameHandle`.
 */
VOID
Kao_findGameProcess(
  LPCTSTR class_name,
  LPCTSTR window_name,
  LPCTSTR exec_prefix)
{
  HANDLE proc_handle;
  HWND glut_window;
  DWORD pids[ENUMERATED_PROCESSES], i, count;
  TCHAR buf[BUF_SIZE];
  LPTSTR filename_a, filename_b;

  /* Find game window */
  glut_window = FindWindow(class_name, window_name);

  if IS_NULL(glut_window)
  {
    _sntprintf(
      g_lastMessage,
      LASTMSG_SIZE,
      TEXT("Could not locate \"%s\" window!"),
      window_name);

    return;
  }

  /* Enumerate processes */
  i = EnumProcesses(
    pids,
    (sizeof(DWORD) * ENUMERATED_PROCESSES),
    &(count));

  if IS_FALSE(i)
  {
    _sntprintf(
      g_lastMessage,
      LASTMSG_SIZE,
      TEXT("EnumProcesses() failed!"));

    return;
  }

  for (i = 0; i < count; i++)
  {
    proc_handle = OpenProcess(
      (
        PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ |
        PROCESS_VM_WRITE |
        PROCESS_VM_OPERATION ),
      FALSE,
      pids[i]);

    if (NOT_NULL(proc_handle) && NOT_INVALID(proc_handle))
    {
      if (Kao_matchGameWindowWithProcess(glut_window, pids[i]))
      {
        buf[0] = TEXT('\0');
        GetModuleFileNameEx(proc_handle, NULL, buf, BUF_SIZE);
        buf[BUF_SIZE - 1] = TEXT('\0');

        /* Find the last part of the filepath */
        filename_a = _tcsrchr(buf, TEXT('\\'));
        filename_b = _tcsrchr(buf, TEXT('/'));

        if (filename_b > filename_a)
        {
          filename_a = filename_b;
        }
        else if IS_NULL(filename_a)
        {
          filename_a = buf - 1;
        }

        filename_a++;

        /* Check if the filename starts with a predefined prefix */
        if EQ(filename_a, _tcsstr(filename_a, exec_prefix))
        {
          g_Kao_gameWindow = glut_window;
          g_Kao_gameHandle = proc_handle;
          return;
        }
      }

      /* Not a valid candidate for the game process */
      CloseHandle(proc_handle);
    }
  }

  _sntprintf(
    g_lastMessage,
    LASTMSG_SIZE,
    TEXT("No process module filename matches the expected executable name!"));
}

/**
 * @brief Prepares "Communicator algorithm" ("hijacking" main game loop).
 *
 * "Communicator algorithm" is used to remotely call specific functions
 * within the game, such as memory allocation or game-object virtual functions.
 * @return TRUE if writing to game memory was successful, FALSE otherwise.
 */
BOOL
Kao_prepareCommunicator()
{
  BOOL return_value = FALSE;
  DWORD dwordTest;

  if EQ(KAO_GAMEVERSION_2PL, g_Kao_gameVersion)
  {
    TRY_PUT_KAOHACK(KAO2PL_COMMHACK02)
    TRY_PUT_KAOHACK(KAO2PL_COMMHACK01)
  }
  else if EQ(KAO_GAMEVERSION_3PL, g_Kao_gameVersion)
  {
    TRY_PUT_KAOHACK(KAO3PL_COMMHACK02)
    TRY_PUT_KAOHACK(KAO3PL_COMMHACK01)
  }
  else
  {
    goto leave;
  }

  KAO_PUT_DWORD(g_KaoOffsets->COMMUNICATOR_ARGS, KAOHACK_COMM_SKIP)
  return_value = TRUE;

  leave: {}
  return return_value;
}


/**************************************************************/
/* Kao :: Public functions                                    */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
Kao_writeMem(DWORD address, LPCVOID from, size_t length)
{
  SIZE_T feedback;

  const BOOL test = WriteProcessMemory(
    g_Kao_gameHandle,
    (LPVOID)address,
    from,
    length,
    &(feedback));

  if (IS_FALSE(test) || NE(feedback, length))
  {
    _sntprintf(
      g_lastMessage,
      LASTMSG_SIZE,
      TEXT("Could not write %u bytes to address 0x%08lX!"),
      length,
      address);

    return FALSE;
  }

  return TRUE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
Kao_readMem(DWORD address, LPVOID into, size_t length)
{
  SIZE_T feedback;

  const BOOL test = ReadProcessMemory(
    g_Kao_gameHandle,
    (LPCVOID)address,
    into,
    length,
    &(feedback));

  if (IS_FALSE(test) || NE(feedback, length))
  {
    _sntprintf(
      g_lastMessage,
      LASTMSG_SIZE,
      TEXT("Could not read %u bytes from address 0x%08lX!"),
      length,
      address);

    return FALSE;
  }

  return TRUE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
Kao_init()
{
  /* Reset global data */

  g_Kao_gameWindow = NULL;
  g_Kao_gameHandle = INVALID_HANDLE_VALUE;

  g_Kao_gameVersion = 0;

  g_Kao_lastLoadingState = FALSE;

  /* Intialize tool-app */

  if IS_FALSE(Kao_initEx())
  {
    return FALSE;
  }

  /* Initialization successful */

  return TRUE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
Kao_startSession(int version)
{
  TCHAR buf[MEDIUM_BUF_SIZE];
  KaoOffsets * kaoOffsetsTest;
  LPCTSTR className;
  LPCTSTR windowName;
  LPCTSTR execPrefix;

  Kao_invalidateSession(TRUE);
  Kao_gameDetachedEx();

  if EQ(KAO_GAMEVERSION_2PL, version)
  {
    kaoOffsetsTest = &(KAO2PL_OFFSETS);
    className  = TEXT(KAO2PL_WND_CLASS);
    windowName = TEXT(KAO2PL_WND_NAME);
    execPrefix = TEXT(KAO2PL_EXEC_PREFIX);
  }
  else if EQ(KAO_GAMEVERSION_3PL, version)
  {
    kaoOffsetsTest = &(KAO3PL_OFFSETS);
    className  = TEXT(KAO3PL_WND_CLASS);
    windowName = TEXT(KAO3PL_WND_NAME);
    execPrefix = TEXT(KAO3PL_EXEC_PREFIX);
  }
  else
  {
    return FALSE;
  }

  Kao_findGameProcess(className, windowName, execPrefix);

  if IS_INVALID(g_Kao_gameHandle)
  {
    _sntprintf(buf, MEDIUM_BUF_SIZE, TEXT("Failed to attach \"%s\"!"), execPrefix);
    GUI_displayError(buf);

    return FALSE;
  }

  g_Kao_gameVersion = version;
  g_KaoOffsets = kaoOffsetsTest;

  if IS_FALSE(Kao_prepareCommunicator())
  {
    Kao_invalidateSession(TRUE);
    return FALSE;
  }

  _sntprintf(buf, MEDIUM_BUF_SIZE, TEXT("\"%s\" attached and ready! ;)"), execPrefix);
  GUI_updateStatus(buf);

  Kao_gameAttachedEx();
  return TRUE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
Kao_invalidateSession(BOOL checkGameHandle)
{
  if (checkGameHandle && NOT_INVALID(g_Kao_gameHandle))
  {
    CloseHandle(g_Kao_gameHandle);
    g_Kao_gameHandle = INVALID_HANDLE_VALUE;
  }
  else
  {
    g_Kao_gameHandle = INVALID_HANDLE_VALUE;
  }

  g_Kao_gameWindow = NULL;

  g_Kao_gameVersion = 0;

  g_KaoOffsets = NULL;

  g_Kao_lastLoadingState = FALSE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
Kao_checkGameState()
{
  BYTE byteTest;
  DWORD dwordTest;

  if IS_INVALID(g_Kao_gameHandle)
  {
    return FALSE;
  }

  if (
    IS_FALSE(GetExitCodeProcess(g_Kao_gameHandle, &(dwordTest))) ||
    NE(STILL_ACTIVE, dwordTest))
  {
    Kao_invalidateSession(FALSE);

    Kao_gameDetachedEx();

    GUI_updateStatus(
      TEXT("Game has been closed!\n" \
      "Please re-launch \"Kao the Kangaroo\"."));

    return FALSE;
  }

  if NOT_NULL(g_KaoOffsets)
  {
    KAO_GET_BYTE(g_KaoOffsets->GLOBAL_LOADING);

    if (IS_FALSE(g_Kao_lastLoadingState) && NOT_FALSE(byteTest))
    {
      g_Kao_lastLoadingState = TRUE;
      Kao_gameStartedLoadingEx();

    }
    else if (NOT_FALSE(g_Kao_lastLoadingState) && IS_FALSE(byteTest))
    {
      g_Kao_lastLoadingState = FALSE;
      Kao_gameFinishedLoadingEx();
    }
  }

  leave: {}

  return (!(g_Kao_lastLoadingState));
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
Kao_end()
{
  Kao_endEx();

  if NOT_INVALID(g_Kao_gameHandle)
  {
    CloseHandle(g_Kao_gameHandle);
  }
}


/**************************************************************/

/**
 * @}
 */


/**************************************************************/
