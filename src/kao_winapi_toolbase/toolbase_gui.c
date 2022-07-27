/**************************************************************/
/* "kao_winapi_toolbase/toolbase_gui.c"                       */
/**************************************************************/

#include "toolbase_gui.h"

/**
 * @addtogroup ToolBase_GUI
 * @{
 */


/**************************************************************/
/* WINAPI GUI :: Including static splash screen               */
/**************************************************************/

/**
 * @def STR(x)
 * Stringify a preprocessor token.
 * @def XSTR(x)
 * Stringify a preprocessor token.
 * @def INC_SPLASH_PATH
 * Path to a file containing data (width, height, pixels) of a Splash Window
 * that will be displayed in some corner of the Main ToolApp Window.
 */

#define STR(x) #x
#define XSTR(x) STR(x)

#ifdef INC_SPLASH_PATH
  #include XSTR(INC_SPLASH_PATH)
#endif


/**************************************************************/
/* WINAPI GUI :: Public data                                  */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
HFONT g_GUI_font;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
HWND g_GUI_windowMain;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
HWND g_GUI_labelStatus;


/**************************************************************/
/* WINAPI GUI :: Private functions                            */
/**************************************************************/

/**
 * @brief Callback procedure for the main window.
 *
 * Function processes standard window messages (opening, closing)
 * and calls `GUI_windowProcedureEx`.
 * @param hWnd Handle to the main window.
 * @param Msg System-defined message to dispatch.
 * @param wParam Message parameter.
 * @param lParam Message parameter.
 * @return Result of message processing.
 */
LRESULT CALLBACK
GUI_windowProcedure(
  HWND hWnd,
  UINT Msg,
  WPARAM wParam,
  LPARAM lParam)
{
  BOOL checkOther = TRUE;
  LRESULT exResult;

  switch (Msg)
  {
    /* Main window destroyed */
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      g_quitTest = TRUE;

      checkOther = FALSE;
      break;
    }

    /* Main window closed */
    case WM_CLOSE:
    {
      DestroyWindow(hWnd);

      checkOther = FALSE;
      break;
    }
  }

  if IS_TRUE(checkOther)
  {
    exResult = GUI_windowProcedureEx(hWnd, Msg, wParam, lParam);

    if NOT_ZERO(exResult)
    {
      return exResult;
    }
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

#ifdef INC_SPLASH_PATH

  /**
   * @brief Callback procedure for the splash sub-window.
   *
   * Function processes standard window messages (painting).
   * @param hWnd Handle to the main window.
   * @param Msg System-defined message to dispatch.
   * @param wParam Message parameter.
   * @param lParam Message parameter.
   * @return Result of message processing.
   */
  LRESULT CALLBACK
  GUI_splashWindowProcedure(
    HWND hWnd,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam)
  {
    BITMAPINFOHEADER bih;
    PAINTSTRUCT ps;
    HDC hdc_paint;

    switch (Msg)
    {
      case WM_PAINT:
      {
        ZeroMemory(&(bih), sizeof(BITMAPINFOHEADER));
        bih.biSize        = sizeof(BITMAPINFOHEADER);
        bih.biBitCount    = 24;
        bih.biPlanes      = 1;
        bih.biCompression = BI_RGB;
        bih.biWidth       = SPLASH_IMAGE_WIDTH;
        bih.biHeight      = SPLASH_IMAGE_HEIGHT;

        hdc_paint = BeginPaint(hWnd, &(ps));

        StretchDIBits(
          hdc_paint,
          0, 0,
          SPLASH_IMAGE_WIDTH, SPLASH_IMAGE_HEIGHT,
          0, 0,
          SPLASH_IMAGE_WIDTH, SPLASH_IMAGE_HEIGHT,
          SPLASH_PIXELS, (BITMAPINFO *) &(bih), DIB_RGB_COLORS,
          SRCCOPY);

        EndPaint(hWnd, &(ps));
        break;
      }
    }

    return DefWindowProc(hWnd, Msg, wParam, lParam);
  }

#endif  /* INC_SPLASH_PATH */

/**
 * @brief Create windows for the application.
 *
 * Function creates all windows (main window, status label, splash image)
 * and calls `GUI_createWindowsEx` (to create tool-app sub-windows).
 * @param hInstance Handle to the current application instance.
 * @param windowTitle Defines the main window title.
 * @param windowWidth Defines main window width in pixels.
 * @param windowHeight Defines main window height in pixels.
 * @return TRUE if windows creation was successful, FALSE otherwise.
 */
BOOL
GUI_createWindows(
  HINSTANCE hInstance,
  LPCTSTR windowTitle,
  DWORD windowWidth,
  DWORD windowHeight)
{
  BOOL return_value = FALSE;

  HWND test_window;
  WNDCLASSEX window_class;
  RECT real_window_rect;

  const int PADDING            =   8;
  const int WINDOW_WIDTH_NOPAD = windowWidth - 2 * PADDING;

  const int FONT_HEIGHT   = 16;
  const int LABEL_HEIGHT  = FONT_HEIGHT + 4;

  LONG i, x, y;

  /* Register Windows */

  ZeroMemory(&(window_class), sizeof(WNDCLASSEX));
  window_class.cbSize = sizeof(WNDCLASSEX);
  window_class.hInstance = hInstance;
  window_class.style = (CS_OWNDC | CS_VREDRAW | CS_HREDRAW);
  window_class.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE);

  window_class.lpfnWndProc = GUI_windowProcedure;
  window_class.lpszClassName = TEXT(GUI_WND_CLASSNAME_A);
  window_class.hIcon = LoadIcon(hInstance, TEXT("MAINICON"));

  TRY_BOOL(RegisterClassEx(&(window_class)))

  #ifdef INC_SPLASH_PATH

    window_class.lpfnWndProc = GUI_splashWindowProcedure;
    window_class.lpszClassName = TEXT(GUI_WND_CLASSNAME_B);
    window_class.hIcon = NULL;

    TRY_BOOL(RegisterClassEx(&(window_class)))

  #endif  /* INC_SPLASH_PATH */

  /* Create fonts */

  g_GUI_font = CreateFont(
    FONT_HEIGHT, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE),
    TEXT("Verdana"));

  TRY_BOOL(g_GUI_font)

  /* Create Main Window */

  real_window_rect.left   = 0;
  real_window_rect.right  = windowWidth;
  real_window_rect.top    = 0;
  real_window_rect.bottom = windowHeight;

  i = (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
  AdjustWindowRect(&real_window_rect, i, FALSE);

  real_window_rect.right  -= real_window_rect.left;
  real_window_rect.bottom -= real_window_rect.top;

  g_GUI_windowMain = CreateWindow(
    TEXT(GUI_WND_CLASSNAME_A), windowTitle, i,
    (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2,
    (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2,
    real_window_rect.right, real_window_rect.bottom,
    NULL, 0, hInstance, NULL);

  GUI_CHECK_WND_FNT(g_GUI_windowMain);

  /* Status label */

  g_GUI_labelStatus = CreateWindowEx(
    WS_EX_CLIENTEDGE, TEXT("STATIC"), TEXT(""),
    WS_VISIBLE | WS_CHILD,
    PADDING, PADDING,
    WINDOW_WIDTH_NOPAD, 2 * LABEL_HEIGHT,
    g_GUI_windowMain, NULL, hInstance, NULL);

  GUI_CHECK_WND_FNT(g_GUI_labelStatus);

  /* Controls for specific tool-app */

  TRY_BOOL(
    GUI_createWindowsEx(hInstance, windowWidth, windowHeight))

  #ifdef INC_SPLASH_PATH

    /* Splash window */

    x = PADDING;
    y = windowHeight - SPLASH_IMAGE_HEIGHT - PADDING;

    test_window = CreateWindowEx(
      WS_EX_CLIENTEDGE, TEXT(GUI_WND_CLASSNAME_B), TEXT(""),
      WS_VISIBLE | WS_CHILD,
      x, y, SPLASH_IMAGE_WIDTH, SPLASH_IMAGE_HEIGHT,
      g_GUI_windowMain, NULL, hInstance, NULL);

    TRY_BOOL(test_window);

  #endif  /* INC_SPLASH_PATH */

  /* Finished */

  GUI_updateStatus(
    TEXT("Welcome! Start by pressing one of the \"Attach\" buttons."));

  ShowWindow(g_GUI_windowMain, SW_SHOW);

  return_value = TRUE;

  leave:
  {}

  return return_value;
}


/**************************************************************/
/* WINAPI GUI :: Public functions                             */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
GUI_updateStatus(LPCTSTR message)
{
  SetWindowText(g_GUI_labelStatus, message);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
GUI_displayError(LPCTSTR caption)
{
  TCHAR buf[BUF_SIZE];

  GUI_updateStatus(caption);

  if NOT_ZERO(g_lastMessage[0])
  {
    _sntprintf(buf, BUF_SIZE, TEXT("%s\n\n%s"), caption, g_lastMessage);

    MessageBox(g_GUI_windowMain, buf, TEXT(GUI_MSGBOX_ERROR), MB_ICONERROR);
  }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
GUI_windowLoop()
{
  BOOL still_active = TRUE;
  MSG message = {0};

  while (PeekMessage(&(message), NULL, 0, 0, PM_REMOVE))
  {
    if EQ(WM_QUIT, message.message)
    {
      still_active = FALSE;
    }

    TranslateMessage(&(message));
    DispatchMessage(&(message));
  }

  return ((!g_quitTest) && still_active);
}

/**
 * @see toolbase_gui.h
 */
BOOL
GUI_init(
  HINSTANCE hInstance,
  LPCTSTR windowTitle,
  DWORD windowWidth,
  DWORD windowHeight)
{
  /* Only one instance at a time! */

  if NOT_NULL(FindWindow(TEXT(GUI_WND_CLASSNAME_A), windowTitle))
  {
    return FALSE;
  }

  /* Reset external data */

  g_GUI_font = NULL;

  g_GUI_windowMain = NULL;
  g_GUI_labelStatus = NULL;

  /* Init for specific tool-app */

  GUI_initEx();

  /* Preparing Graphical User Interface */

  if IS_FALSE(
    GUI_createWindows(
      hInstance,
      windowTitle,
      windowWidth,
      windowHeight))
  {
    MessageBox(
      NULL,
      TEXT("Could not create the application window!"),
      TEXT(GUI_MSGBOX_ERROR),
      MB_ICONERROR);

    return FALSE;
  }

  /* Initialization successful */

  return TRUE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
GUI_end()
{
  /* End for specific tool-app */

  GUI_endEx();

  /* Freeing memory of some global objects */

  if NOT_NULL(g_GUI_font)
  {
    DeleteObject((HGDIOBJ)g_GUI_font);
  }
}


/**************************************************************/

/**
 * @}
 */


/**************************************************************/
