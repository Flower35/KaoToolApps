/**************************************************************/
/* "kao2_xray/xray_gui.h"                                     */
/**************************************************************/

#ifndef H_KAO2_XRAY_GUI
#define H_KAO2_XRAY_GUI

#include "../kao_winapi_toolbase/toolbase_gui.h"

#include "xray_main.h"

/**
 * @defgroup KaoToolApp_XRay_GUI Kao Tool App :: XRay GUI
 * @{
 */


/**************************************************************/
/* C Libraries                                                */
/**************************************************************/

#include <commdlg.h>


/**************************************************************/
/* Kao2 XRay GUI :: Constants                                 */
/**************************************************************/

/**
 * @def GUI_WND_LABEL
 * Title of the window created for this ToolApp.
 * @def GUI_WND_WIDTH
 * ToolApp window width (in pixels).
 * @def GUI_WND_HEIGHT
 * ToolApp window height (in pixels).
 * @def GUI_CHECKBOX_NAME_INVISIBLE
 * Label for one of the CheckBoxes.
 * @def GUI_CHECKBOX_NAME_TRIGGERS
 * Label for one of the CheckBoxes.
 * @def GUI_DEFAULT_INVISIBLE_COLOR
 * Default color for one of the materials.
 * @def GUI_DEFAULT_TRIGGERS_COLOR
 * Default color for one of the materials.
 */

#define GUI_WND_LABEL   "Kao2 :: XRay Tool"
#define GUI_WND_WIDTH   640
#define GUI_WND_HEIGHT  360

#define GUI_CHECKBOX_NAME_INVISIBLE  "Show invisible meshes with collision"
#define GUI_CHECKBOX_NAME_TRIGGERS   "Show box zones (triggers)"

#define GUI_DEFAULT_INVISIBLE_COLOR  RGB(  0,128,255)
#define GUI_DEFAULT_TRIGGERS_COLOR   RGB(255, 64, 64)


/**************************************************************/
/* Kao2 XRay GUI :: Public data                               */
/**************************************************************/

/**
 * @brief Handles to the "Attach <Kao>" Buttons.
 */
extern HWND g_XRayGUI_buttonsAttach[2];

/**
 * @brief Handles to the XRay CheckBoxes.
 */
extern HWND g_XRayGUI_checkBoxes[2];

/**
 * @brief Handles to the static windows containing the color
 * in which invisible meshes will be marked with.
 */
extern HWND g_XRayGUI_staticColors[2];

/**
 * @brief Global COLORREF array for `ChooseColor()` function.
 */
extern COLORREF g_XRayGUI_customColors[16];

/**
 * @brief Global COLORREF variables representing the color
 * in which invisible meshes will be marked with.
 */
extern COLORREF g_XRayGUI_meshColors[2];

/**
 * @brief Global HBRUSH handles to the background color
 * for `g_XRayGui_staticColors` windows.
 */
extern HBRUSH g_XRayGUI_brushColors[2];

/**
 * @brief Dynamic images created with `g_XRayGUI_meshColors`.
 */
extern LPDWORD g_XRayGUI_rgbaImages[2];


/**************************************************************/
/* Kao2 XRay GUI :: Public functions                          */
/**************************************************************/

/**
 * @brief Unchecks "invisible meshes" and "box zones" CheckBoxes.
 **/
VOID KaoXRayGUI_clear();


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO2_XRAY_GUI */


/**************************************************************/
