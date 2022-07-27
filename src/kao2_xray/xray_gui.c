/**************************************************************/
/* "kao2_xray/xray_gui.c"                                     */
/**************************************************************/

#include "xray_gui.h"
#include "xray_nodes.h"

#include "../kao_winapi_toolbase/toolbase_misc.h"

/**
 * @addtogroup KaoToolApp_XRay_GUI
 * @{
 */


/**************************************************************/
/* Kao2 XRay GUI :: Public data                               */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
HWND g_XRayGUI_buttonsAttach[2];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
HWND g_XRayGUI_checkBoxes[2];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
HWND g_XRayGUI_staticColors[2];

COLORREF g_XRayGUI_customColors[16];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
COLORREF g_XRayGUI_meshColors[2];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
HBRUSH g_XRayGUI_brushColors[2];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
LPDWORD g_XRayGUI_rgbaImages[2];


/**************************************************************/
/* Kao2 XRay GUI :: Public functions                          */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
KaoXRayGUI_clear()
{
  SendMessage(
    g_XRayGUI_checkBoxes[0],
    BM_SETCHECK,
    (WPARAM) BST_UNCHECKED,
    (LPARAM) 0);

  SendMessage(
    g_XRayGUI_checkBoxes[1],
    BM_SETCHECK,
    (WPARAM) BST_UNCHECKED,
    (LPARAM) 0);
}


/**************************************************************/
/* WINAPI GUI :: Special public functions                     */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
LRESULT
GUI_windowProcedureEx(
  HWND hWnd,
  UINT Msg,
  WPARAM wParam,
  LPARAM lParam)
{
  const HWND hwndCtrl = (HWND) lParam;
  CHOOSECOLOR cc;
  DWORD i = 0;
  BOOL toggledOn;
  HDC hdcStatic;

  switch (Msg)
  {
    /* Drawing some static window */
    case WM_CTLCOLORSTATIC:
    {
      hdcStatic = (HDC) wParam;

      if EQ(g_XRayGUI_staticColors[0], hwndCtrl)
      {
        SetBkColor(hdcStatic, g_XRayGUI_meshColors[0]);
        return (LRESULT) g_XRayGUI_brushColors[0];
      }
      else if EQ(g_XRayGUI_staticColors[1], hwndCtrl)
      {
        SetBkColor(hdcStatic, g_XRayGUI_meshColors[1]);
        return (LRESULT) g_XRayGUI_brushColors[1];
      }
      else
      {
        return FALSE;
      }

      break;
    }

    /* Command from some child control */
    case WM_COMMAND:
    {
      if GUI_BUTTON_CLICKED(g_XRayGUI_buttonsAttach[0])
      {
        g_controlFlags = XRAY_ACT_ATTACH_KAO2;
      }
      else if GUI_BUTTON_CLICKED(g_XRayGUI_buttonsAttach[1])
      {
        g_controlFlags = XRAY_ACT_ATTACH_KAO3;
      }
      else if GUI_BUTTON_CLICKED(g_XRayGUI_checkBoxes[0])
      {
        toggledOn = (BOOL) SendMessage((HWND) lParam, BM_GETCHECK, 0, 0);
        g_controlFlags = MAKELONG(XRAY_ACT_TOGGLE_INVIS, toggledOn);
      }
      else if GUI_BUTTON_CLICKED(g_XRayGUI_checkBoxes[1])
      {
        toggledOn = (BOOL) SendMessage((HWND) lParam, BM_GETCHECK, 0, 0);
        g_controlFlags = MAKELONG(XRAY_ACT_TOGGLE_TRIGG, toggledOn);
      }
      else if GUI_STATIC_CLICKED(g_XRayGUI_staticColors[0])
      {
        i = 1;
      }
      else if GUI_STATIC_CLICKED(g_XRayGUI_staticColors[1])
      {
        i = 2;
      }

      break;
    }
  }

  if NOT_ZERO(i)
  {
    i--;
    ZeroMemory(&(cc), sizeof(CHOOSECOLOR));
    cc.lStructSize  = sizeof(CHOOSECOLOR);
    cc.hwndOwner    = hWnd;
    cc.rgbResult    = g_XRayGUI_meshColors[i];
    cc.lpCustColors = (COLORREF *) g_XRayGUI_customColors;
    cc.Flags        = (CC_RGBINIT | CC_FULLOPEN);

    if NOT_FALSE(ChooseColor(&cc))
    {
      g_XRayGUI_meshColors[i] = cc.rgbResult;

      if NOT_NULL(g_XRayGUI_brushColors[i])
      {
        DeleteObject((HGDIOBJ) g_XRayGUI_brushColors[i]);
      }
      g_XRayGUI_brushColors[i] = CreateSolidBrush(cc.rgbResult);

      InvalidateRect(hWnd, NULL, FALSE);

      if NOT_NULL(g_XRayGUI_rgbaImages[i])
      {
        free(g_XRayGUI_rgbaImages[i]);
      }
      g_XRayGUI_rgbaImages[i] = Misc_generateCheckerImage(
        cc.rgbResult,
        MISC_DEFAULT_IMAGE_WIDTH, MISC_DEFAULT_IMAGE_WIDTH,
        (1.0f /  2), (1.0f / (8 * (i + 1))));

      KaoXRayNodes_replaceBitmapInTexture(i);
    }
  }

  return FALSE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
GUI_createWindowsEx(
  HINSTANCE hInstance,
  DWORD windowWidth,
  DWORD windowHeight)
{
  BOOL return_value = FALSE;

  const int PADDING         =   8;
  const int WINDOW_HW       = windowWidth / 2;
  const int WINDOW_HW_NOPAD = WINDOW_HW - 2 * PADDING;

  const int FONT_HEIGHT   = 16;
  const int LABEL_HEIGHT  = FONT_HEIGHT + 4;
  const int BUTTON_HEIGHT = 32;

  /* Action Buttons, Labels, EditBoxes */

  GUI_PLACE_PSHBTN(g_XRayGUI_buttonsAttach[0], 0, 0, TEXT("Attach <Kangurek Kao: Runda 2>"), NULL)
  GUI_PLACE_PSHBTN(g_XRayGUI_buttonsAttach[1], 0, 1, TEXT("Attach <Kao: Tajemnica Wulkanu>"), NULL)

  GUI_PLACE_SQUARE(g_XRayGUI_staticColors[0],  3, 8, TEXT(""), NULL);
  GUI_PLACE_CHKBTN(g_XRayGUI_checkBoxes[0],    5, 1, TEXT(GUI_CHECKBOX_NAME_INVISIBLE), NULL)

  GUI_PLACE_SQUARE(g_XRayGUI_staticColors[1],  8, 8, TEXT(""), NULL);
  GUI_PLACE_CHKBTN(g_XRayGUI_checkBoxes[1],   10, 1, TEXT(GUI_CHECKBOX_NAME_TRIGGERS), NULL)

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
GUI_initEx()
{
  DWORD i;
  const COLORREF colorA = GUI_DEFAULT_INVISIBLE_COLOR;
  const COLORREF colorB = GUI_DEFAULT_TRIGGERS_COLOR;

  /* Reset external data */

  g_XRayGUI_buttonsAttach[0] = NULL;
  g_XRayGUI_buttonsAttach[1] = NULL;

  g_XRayGUI_checkBoxes[0] = NULL;
  g_XRayGUI_checkBoxes[1] = NULL;

  g_XRayGUI_staticColors[0] = NULL;
  g_XRayGUI_staticColors[1] = NULL;

  for (i = 0; i < 16; i++)
  {
    g_XRayGUI_customColors[i] = RGB(255,255,255);
  }

  g_XRayGUI_meshColors[0] = colorA;
  g_XRayGUI_meshColors[1] = colorB;

  g_XRayGUI_brushColors[0] = CreateSolidBrush(colorA);
  g_XRayGUI_brushColors[1] = CreateSolidBrush(colorB);

  g_XRayGUI_rgbaImages[0] = Misc_generateCheckerImage(
    colorA,
    MISC_DEFAULT_IMAGE_WIDTH, MISC_DEFAULT_IMAGE_WIDTH,
    (1.0f /  2), (1.0f /  8));

  g_XRayGUI_rgbaImages[1] = Misc_generateCheckerImage(
    colorB,
    MISC_DEFAULT_IMAGE_WIDTH, MISC_DEFAULT_IMAGE_WIDTH,
    (1.0f /  2), (1.0f / 16));
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
GUI_endEx()
{
  if NOT_NULL(g_XRayGUI_brushColors[0])
  {
    DeleteObject((HGDIOBJ) g_XRayGUI_brushColors[0]);
  }

  if NOT_NULL(g_XRayGUI_brushColors[1])
  {
    DeleteObject((HGDIOBJ) g_XRayGUI_brushColors[1]);
  }

  if NOT_NULL(g_XRayGUI_rgbaImages[0])
  {
    free(g_XRayGUI_rgbaImages[0]);
  }

  if NOT_NULL(g_XRayGUI_rgbaImages[1])
  {
    free(g_XRayGUI_rgbaImages[1]);
  }
}


/**************************************************************/

/**
 * @}
 */


/**************************************************************/
