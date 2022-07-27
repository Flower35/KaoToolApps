/**************************************************************/
/* "kao2_xray/xray_nodes.h"                                   */
/**************************************************************/

#ifndef H_KAO2_XRAY_NODES
#define H_KAO2_XRAY_NODES

#include "xray_main.h"

#include "../kao_winapi_toolbase/toolbase_anyarray.h"

/**
 * @defgroup KaoToolApp_XRay_Nodes Kao Tool App :: XRay Nodes
 * @{
 */


/**************************************************************/
/* Kao2 XRay Nodes :: Public data                             */
/**************************************************************/

/**
 * @brief Array that holds references to `eTriMesh`
 * nodes that were invisible.
 */
extern AnyArray g_KaoXRayNodes_arrayInvisibleMeshes;

/**
 * @brief Array that holds references to `eMaterial`
 * game-objects with different collision.
 */
extern AnyArray g_KaoXRayNodes_arrayMaterials;

/**
 * @brief Array that holds references to `eTriMesh`
 * nodes generated next to `eBoxZone` game-objects.
 */
extern AnyArray g_KaoXRayNodes_arrayTriggerMeshes;

/**
 * @brief `eTexture` game-objects that hold the reference
 * to `eBitmap` generated from `g_XRayGUI_meshColors`.
 */
extern DWORD g_KaoXRayNodes_magicTextures[2];


/**************************************************************/
/* Kao2 XRay Nodes :: Public functions                   */
/**************************************************************/

/**
 * @brief Initializes global compontents of this module.
 **/
VOID KaoXRayNodes_init();

/**
 * @brief Destroys global components of this module.
 **/
VOID KaoXRayNodes_clear();

/**
 * @brief Replaces `eBitmap` game-objects by generating new bitmaps
 * from a new set of pixels.
 *
 * @param idx Index of the `eTexture` game-object
 * in `g_KaoXRayNodes_magicTextures` global array.
 **/
VOID KaoXRayNodes_replaceBitmapInTexture(const DWORD idx);

/**
 * @brief Action that toggles invisible collision meshes.
 *
 * When this function is called for the first time, it will traverse
 * all scene-nodes and copy game-object addresses to a cache-array.
 * @param toggledOn Should the nodes be turned ON or OFF.
 * @return TRUE on success, FALSE on I/O (or memory allocation) error.
 **/
BOOL KaoXRayNodes_toggleInvisibleCollision(BOOL toggledOn);

/**
 * @brief Action that toggles BoxZone Triggers
 *
 * When this function is called for the first time, it will traverse
 * all scene-nodes and copy game-object addresses to a cache-array.
 * @param toggledOn Should the nodes be turned ON or OFF.
 * @return TRUE on success, FALSE on I/O (or memory allocation) error.
 **/
BOOL KaoXRayNodes_toggleBoxZones(BOOL toggledOn);


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO2_XRAY_NODES */


/**************************************************************/
