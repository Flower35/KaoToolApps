/**************************************************************/
/* "kao2_xray/xray_main.h"                                    */
/**************************************************************/

#ifndef H_KAO2_XRAY_MAIN
#define H_KAO2_XRAY_MAIN

#include "../kao_winapi_toolbase/toolbase_main.h"

/**
 * @defgroup KaoToolApp_XRay_Main Kao Tool App :: XRay
 * @{
 */


/**************************************************************/
/* Kao2 X-Ray Tool :: Constants                               */
/**************************************************************/

/**
 * @def XRAY_ACT_NONE
 * Do nothing (skip this loop).
 * @def XRAY_ACT_ATTACH_KAO2
 * "Attach <kao2>" button was pressed.
 * @def XRAY_ACT_ATTACH_KAO3
 * "Attach <kao_tw>" button was pressed.
 * @def XRAY_ACT_TOGGLE_INVIS
 * "Show invisible meshes" CheckBox was toggled ON or OFF.
 * @def XRAY_ACT_TOGGLE_TRIGG
 * "Show BoxZone triggers" CheckBox was toggled ON or OFF.
 */

#define XRAY_ACT_NONE          0
#define XRAY_ACT_ATTACH_KAO2   1
#define XRAY_ACT_ATTACH_KAO3   2
#define XRAY_ACT_TOGGLE_INVIS  3
#define XRAY_ACT_TOGGLE_TRIGG  4


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO2_XRAY_MAIN */


/**************************************************************/
