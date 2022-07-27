/**************************************************************/
/* "kao_winapi_toolbase/toolbase_gui.h"                       */
/**************************************************************/

#ifndef H_KAO_WINAPI_TOOLBASE_GUI
#define H_KAO_WINAPI_TOOLBASE_GUI

#include "toolbase_main.h"

/**
 * @defgroup ToolBase_GUI ToolBase :: GUI
 * @{
 */


/**************************************************************/
/* WINAPI GUI :: Constants                                    */
/**************************************************************/

/**
 * @def GUI_WND_CLASSNAME_A
 * Unique class name for a registered WinApi window class.
 * @def GUI_WND_CLASSNAME_B
 * Unique class name for a registered WinApi window class.
 * @def GUI_MSGBOX_ERROR
 * Text that appears as the title of error MessageBoxes.
 */

#define GUI_WND_CLASSNAME_A  "KAOTOOL_MAIN_WND_CLASS"
#define GUI_WND_CLASSNAME_B  "KAOTOOL_SPLASH_WND_CLASS"

#define GUI_MSGBOX_ERROR  "Kao Tool App :: Error"


/**************************************************************/
/* WINAPI GUI :: Helper macros                                */
/**************************************************************/

/**
 * @def GUI_BUTTON_CLICKED(__hwnd)
 * Expression that evaluates to TRUE if `hwndCtrl` (local window handle)
 * is the same as `__hwnd` macro parameter, and that Button sub-window
 * sent "just clicked" message to the main window.
 * @def GUI_STATIC_CLICKED(__hwnd)
 * Expression that evaluates to TRUE if `hwndCtrl` (local window handle)
 * is the same as `__hwnd` macro parameter, and that Static sub-window
 * sent "just clicked" message to the main window.
 * @def GUI_CHECK_WND_FNT(__hwnd)
 * Checks if a given Window Handle is valid, then applies a global font to it.
 * @def GUI_PLACE_BUTTON(__hwnd, __style, __row, __col, __text, __idm)
 * Creates "BUTTON" sub-window (with given text, some specified window style)
 * at given grid-coordinates.
 * @def GUI_PLACE_PSHBTN(__hwnd, __row, __col, __text, __idm)
 * Creates a Pushable Button (with given text) at given grid-coordinates.
 * @def GUI_PLACE_CHKBTN(__hwnd, __row, __col, __text, __idm)
 * Creates a CheckBox (with given text) at given grid-coordinates.
 * @def GUI_PLACE_SQUARE(__hwnd, __row, __col, __text, __idm)
 * Creates "STATIC" sub-window (with sunken-edge, that is clickable,
 * has some text) at given grid-coordinates.
 * @def GUI_PLACE_LABEL(__hwnd, __row, __col, __text, __idm)
 * Creates "STATIC" sub-window (with sunken-edge, that has some text)
 * at given grid-coordinates.
 * @def GUI_PLACE_EDIT(__hwnd, __row, __col, __text, __idm)
 * Creates EditBox (with some initial text) at given grid-coordinates.
 * @def GUI_COMBO_ADD(__hwnd, __text)
 * Inserts text line at the end of given ComboBox window.
 */

#define GUI_BUTTON_CLICKED(__hwnd) \
  (EQ(__hwnd, hwndCtrl) && EQ(BN_CLICKED, HIWORD(wParam)))

#define GUI_STATIC_CLICKED(__hwnd) \
  (EQ(__hwnd, hwndCtrl) && EQ(STN_CLICKED, HIWORD(wParam)))

#define GUI_CHECK_WND_FNT(__hwnd) \
  TRY_BOOL(__hwnd) \
  SendMessage(__hwnd, WM_SETFONT, (WPARAM) g_GUI_font, (LPARAM) 0);

#define GUI_PLACE_BUTTON(__hwnd, __style, __row, __col, __text, __idm) \
  __hwnd = CreateWindow( \
    TEXT("BUTTON"), __text, \
    WS_VISIBLE | WS_CHILD | __style, \
    (PADDING + __col * (WINDOW_HW_NOPAD + PADDING)), \
    (2 * PADDING + 2 * LABEL_HEIGHT + __row * LABEL_HEIGHT), \
    WINDOW_HW_NOPAD, BUTTON_HEIGHT, \
    g_GUI_windowMain, (HMENU)__idm, hInstance, NULL); \
  GUI_CHECK_WND_FNT(__hwnd)

#define GUI_PLACE_PSHBTN(__hwnd, __row, __col, __text, __idm) \
  GUI_PLACE_BUTTON(__hwnd, BS_DEFPUSHBUTTON, __row, __col, __text, __idm)

#define GUI_PLACE_CHKBTN(__hwnd, __row, __col, __text, __idm) \
  GUI_PLACE_BUTTON(__hwnd, BS_AUTOCHECKBOX, __row, __col, __text, __idm)

#define GUI_PLACE_SQUARE(__hwnd, __row, __col, __text, __idm) \
  __hwnd = CreateWindowEx( \
    WS_EX_CLIENTEDGE, TEXT("STATIC"), __text, \
    WS_VISIBLE | WS_CHILD | SS_NOTIFY, \
    (PADDING + __col * (2 * LABEL_HEIGHT + PADDING)), \
    (2 * PADDING + 2 * LABEL_HEIGHT + __row * LABEL_HEIGHT), \
    (4 * LABEL_HEIGHT), (2 * LABEL_HEIGHT), \
    g_GUI_windowMain, (HMENU)__idm, hInstance, NULL); \
  GUI_CHECK_WND_FNT(__hwnd)

#define GUI_PLACE_LABEL(__hwnd, __row, __col, __text, __idm) \
  __hwnd = CreateWindowEx( \
    WS_EX_CLIENTEDGE, TEXT("STATIC"), __text, \
    WS_VISIBLE | WS_CHILD, \
    (PADDING + __col * (WINDOW_HW_NOPAD + PADDING)), \
    (2 * PADDING + 2 * LABEL_HEIGHT + __row * LABEL_HEIGHT), \
    WINDOW_HW_NOPAD, LABEL_HEIGHT, \
    g_GUI_windowMain, (HMENU)__idm, hInstance, NULL); \
  GUI_CHECK_WND_FNT(__hwnd)

#define GUI_PLACE_EDIT(__hwnd, __row, __col, __text, __idm) \
  __hwnd = CreateWindowEx( \
    WS_EX_CLIENTEDGE, TEXT("EDIT"), __text, \
    WS_VISIBLE | WS_CHILD, \
    (PADDING + __col * (WINDOW_HW_NOPAD + PADDING)), \
    (2 * PADDING + 2 * LABEL_HEIGHT + __row * LABEL_HEIGHT), \
    WINDOW_HW_NOPAD, LABEL_HEIGHT, \
    g_GUI_windowMain, (HMENU)__idm, hInstance, NULL); \
  GUI_CHECK_WND_FNT(__hwnd)

#define GUI_COMBO_ADD(__hwnd, __text) \
  SendMessage(__hwnd, CB_ADDSTRING, 0, (LPARAM)__text);


/**************************************************************/
/* WINAPI GUI :: Public data                                  */
/**************************************************************/

/**
 * @brief Handle to the font object.
 */
extern HFONT g_GUI_font;

/**
 * @brief Handle to the main window.
 */
extern HWND g_GUI_windowMain;

/**
 * @brief Handle to the status label window.
 */
extern HWND g_GUI_labelStatus;


/**************************************************************/
/* WINAPI GUI :: Public functions                             */
/**************************************************************/

/**
 * @brief Displays message box with an error.
 *
 * Displays MessageBox with set caption (plus error stack trace)
 * and updates status-label to the same caption.
 * @param caption Message that indicates the cause of an error.
 */
VOID GUI_displayError(LPCTSTR caption);

/**
 * @brief Sets status-label text.
 *
 * Replaces text of the status-label window (visible at the top
 * of the main window).
 * @param message Message to be placed inside the status-label.
 */
VOID GUI_updateStatus(LPCTSTR message);

/**
 * @brief Main window loop.
 *
 * Function processes typical Windows events
 * (translating and dispatching messages).
 * @return TRUE if the application is still running.
 */
BOOL GUI_windowLoop();

/**
 * @brief Initializing global data and creating all windows.
 *
 * @return TRUE if initialization was successful, FALSE otherwise.
 */
BOOL GUI_init(
  HINSTANCE hInstance,
  LPCTSTR windowTitle,
  DWORD windowWidth,
  DWORD windowHeight);

/**
 * @brief Cleaning public global data.
 */
VOID GUI_end();


/**************************************************************/
/* WINAPI GUI :: Public functions that require definitions    */
/**************************************************************/

/**
 * @brief Callback prodecure called from `GUI_windowProcedure`.
 *
 * Private callback procedure `GUI_windowProcedure` calls this function.
 *
 * @note This function's body must be defined in a specific tool-app.
 *
 * @param hWnd Handle to the main window.
 * @param Msg System-defined message to dispatch.
 * @param wParam Message parameter.
 * @param lParam Message parameter.
 * @return Result of message processing, passed to the main window procedure.
 */
LRESULT GUI_windowProcedureEx(
  HWND hWnd,
  UINT Msg,
  WPARAM wParam,
  LPARAM lParam);

/**
 * @brief Creates sub-windows for a specific tool-app.
 *
 * Called from `GUI_createWindows()`.
 *
 * @note This function's body must be definied in a specific tool-app.
 *
 * @param hInstance Handle to the current application instance.
 * @param windowWidth Defines main window width in pixels.
 * @param windowHeight Defines main window height in pixels.
 * @return TRUE if sub-windows creation was successful, FALSE otherwise.
 */
BOOL GUI_createWindowsEx(
  HINSTANCE hInstance,
  DWORD windowWidth,
  DWORD windowHeight);

/**
 * @brief Initializing private global data.
 *
 * Called from `GUI_init()`.
 *
 * @note This function's body must be definied in a specific tool-app.
 */
VOID GUI_initEx();

/**
 * @brief Cleaning private global data.
 *
 * Called from `GUI_end()`.
 *
 * @note This function's body must be definied in a specific tool-app.
 */
VOID GUI_endEx();


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO_WINAPI_TOOLBASE_GUI */


/**************************************************************/
