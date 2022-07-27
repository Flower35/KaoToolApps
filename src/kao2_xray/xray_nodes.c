/**************************************************************/
/* "kao2_xray/xray_nodes.c"                                   */
/**************************************************************/

#include "xray_nodes.h"
#include "xray_gui.h"

#include "../kao_winapi_toolbase/toolbase_misc.h"
#include "../kao_winapi_toolbase/toolbase_kao.h"
#include "../kao_winapi_toolbase/toolbase_kao_mem.h"
#include "../kao_winapi_toolbase/toolbase_kao_hacks.h"
#include "../kao_winapi_toolbase/toolbase_kao_objects.h"
#include "../kao_winapi_toolbase/toolbase_anyarray.h"

/**
 * @addtogroup KaoToolApp_XRay_Nodes
 * @{
 */


/**************************************************************/
/* Kao2 XRay Nodes :: Public data                             */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
AnyArray g_KaoXRayNodes_arrayInvisibleMeshes;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
AnyArray g_KaoXRayNodes_arrayMaterials;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
AnyArray g_KaoXRayNodes_arrayTriggerMeshes;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
DWORD g_KaoXRayNodes_magicTextures[2];


/**************************************************************/
/* Kao2 XRay Nodes :: Private functions                       */
/**************************************************************/


/**
 * @brief Traverse scene nodes to find invisible collision meshes,
 * then store them in a global cache array.
 *
 * @param callerPropagateVisibility Pointer to the `localPropagateVisibility`
 * variable of the caller. Set to TRUE when some child node was identified
 * as an invisible collision mesh.
 * @param objectBase Base address of current `eGroup`-derived game object.
 * @return FALSE for error propagation, TRUE if no error was detected.
 */
BOOL
KaoXRayNodes_invisibleCollisionTraverse(
  LPBOOL callerPropagateVisibility,
  const DWORD objectBase)
{
  BOOL return_value = FALSE;
  DWORD i;
  DWORD dwordTest;
  BOOL localPropagateVisibility = FALSE;

  DWORD childrenCount;
  DWORD childrenArray;

  /* Check VPTR */

  KAO_GET_DWORD(objectBase)

  if (
    EQ(g_KaoOffsets->VPTR_GROUP, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_ENVIRONMENT, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_TRANSFORM, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_BILLBOARD, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_PROXY, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_PIVOT, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_ACTOR, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_WSGROUP, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_SCENE, dwordTest))
  {
    /* "eGroup" object */

    KAO_GET_DWORD(objectBase + g_KaoOffsets->GROUP_NODES)
    childrenCount = dwordTest;

    KAO_GET_DWORD(objectBase + g_KaoOffsets->GROUP_NODES + 0x08)
    childrenArray = dwordTest;

    for (i = 0; i < childrenCount; i++)
    {
      KAO_GET_DWORD(childrenArray)

      TRY_BOOL(
        KaoXRayNodes_invisibleCollisionTraverse(
          &(localPropagateVisibility),
          dwordTest))

      if (localPropagateVisibility)
      {
        /* Causes "WS mesh buffer DP2 overrun" error */

        // (...) KAO_GET_DWORD(objectBase + g_KaoOffsets->NODE_FLAGS)
        // (...) dwordTest |= 0x20000008;
        // (...) KAO_PUT_DWORD((objectBase + g_KaoOffsets->NODE_FLAGS), dwordTest);

        if NOT_NULL(callerPropagateVisibility)
        {
          (*callerPropagateVisibility) = TRUE;
        }

        localPropagateVisibility = FALSE;
      }

      childrenArray += 0x04;
    }
  }
  else if EQ(g_KaoOffsets->VPTR_TRIMESH, dwordTest)
  {
    /* "eTriMesh" object */

    /* Check if "Axis List Box" reference is defined */

    KAO_GET_DWORD(objectBase + g_KaoOffsets->NODE_ALBOX)
    if IS_ZERO(dwordTest)
    {
      return TRUE;
    }

    KAO_GET_DWORD(objectBase + g_KaoOffsets->NODE_FLAGS)

    /* Meshes are invisible if flag: */
    /* - 0x00010000 [???] is set (takes priority) */
    /* - 0x00000001 [Node Enabled] is not set */
    /* - 0x00000008 [Node Shown] might not be checked */
    if (
      NE(0x00010000, (0x00010000 & dwordTest)) &&
      EQ(0x00000001, (0x00000001 & dwordTest)))
    {
      return TRUE;
    }

    /* Switching node flags to make `eTriMesh` visible: */
    /* - set 0x00000001 [Node Enabled] */
    /* - set 0x20000000 [Draw Pass #2] */
    /* - clear 0x00010000 [???] */
    /* - clear 0x10000000 [Draw Pass #1] */
    dwordTest |= 0x20000001;
    dwordTest &= (~0x10010000);
    KAO_PUT_DWORD((objectBase + g_KaoOffsets->NODE_FLAGS), dwordTest);

    /* Replacing `eMaterial` reference */
    TRY_BOOL(
      KaoObjects_assignCompatibleMaterial(
        objectBase,
        &(g_KaoXRayNodes_arrayMaterials),
        g_KaoXRayNodes_magicTextures[0]))

    /* Add object to storage (as long as the level is loaded) */
    TRY_BOOL(
      AnyArray_append(
        &(g_KaoXRayNodes_arrayInvisibleMeshes),
        &(objectBase)));

    if NOT_NULL(callerPropagateVisibility)
    {
      (*callerPropagateVisibility) = TRUE;
    }
  }

  return_value = TRUE;
  leave: {}
  return return_value;
}

/**
 * @brief Traverse scene nodes to find BoxZone triggers,
 * then store them in a global cache array.
 *
 * @param callerTriMeshObject Pointer to the `localTriMeshObject` variable
 * of the caller. Set to non-zero when some child node was identified
 * as `eBoxZone` and a proper `eTriMesh` game-object was generated.
 * @param callerPropagateVisibility Pointer to the `localPropagateVisibility`
 * variable of the caller. Set to TRUE when some child node was identified
 * as an invisible collision mesh.
 * @param objectBase Base address of current `eGroup`-derived game object.
 * @param materialObject Address of a generated `eMaterial` game-object
 * that will be assigned to each new `eTriMesh` game-object.
 * @param stripsPortionsObject Address of a generated `eGeoArray<ushort>`
 * game-object that will be assigned to each new `eGeoSet` game-object.
 * @param indicesArrayObject Address of a generated `eGeoArray<ushort>`
 * game-object that will be assigned to each new `eGeoSet` game-object.
 * @param texCoordsArrayObject Address of a generated `eGeoArray<ePoint2>`
 * game-object that will be assigned to each new `eGeoSet` game-object.
 * @param colorsArrayObject Address of a generated `eGeoArray<ePoint4>`
 * game-object that will be assigned to each new `eGeoSet` game-object.
 * @return FALSE for error propagation, TRUE if no error was detected.
 */
BOOL
KaoXRayNodes_boxZonesTraverse(
  LPDWORD callerTriMeshObject,
  LPBOOL callerPropagateVisibility,
  const DWORD objectBase,
  const DWORD materialObject,
  const DWORD stripsPortionsObject,
  const DWORD indicesArrayObject,
  const DWORD texCoordsArrayObject,
  const DWORD colorsArrayObject)
{
  BOOL return_value = FALSE;
  DWORD i;
  DWORD dwordTest;

  ePoint3 minPoint;
  ePoint3 maxPoint;
  DWORD localTriMeshObject = 0;
  BOOL localPropagateVisibility = FALSE;

  DWORD childrenCount;
  DWORD childrenArray;

  /* Check VPTR */

  KAO_GET_DWORD(objectBase)

  if (
    EQ(g_KaoOffsets->VPTR_GROUP, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_ENVIRONMENT, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_TRANSFORM, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_BILLBOARD, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_PROXY, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_PIVOT, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_ACTOR, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_WSGROUP, dwordTest) ||
    EQ(g_KaoOffsets->VPTR_SCENE, dwordTest))
  {
    /* "eGroup" object */

    KAO_GET_DWORD(objectBase + g_KaoOffsets->GROUP_NODES)
    childrenCount = dwordTest;

    KAO_GET_DWORD(objectBase + g_KaoOffsets->GROUP_NODES + 0x08)
    childrenArray = dwordTest;

    for (i = 0; i < childrenCount; i++)
    {
      KAO_GET_DWORD(childrenArray)

      TRY_BOOL(
        KaoXRayNodes_boxZonesTraverse(
          &(localTriMeshObject),
          &(localPropagateVisibility),
          dwordTest,
          materialObject,
          stripsPortionsObject,
          indicesArrayObject,
          texCoordsArrayObject,
          colorsArrayObject));

      if (localPropagateVisibility)
      {
        KAO_GET_DWORD(objectBase + g_KaoOffsets->NODE_FLAGS)
        dwordTest |= 0x20000008;
        KAO_PUT_DWORD((objectBase + g_KaoOffsets->NODE_FLAGS), dwordTest);

        if NOT_NULL(callerPropagateVisibility)
        {
          (*callerPropagateVisibility) = TRUE;
        }

        localPropagateVisibility = FALSE;
      }

      if (localTriMeshObject)
      {
        TRY_BOOL(KaoObjects_attachNode(objectBase, localTriMeshObject))

        TRY_BOOL(KaoObjects_derefRefObj(0, localTriMeshObject))

        localTriMeshObject = 0;
      }

      childrenArray += 0x04;
    }
  }
  else if EQ(g_KaoOffsets->VPTR_BOXZONE, dwordTest)
  {
    /* "eBoxZone" object */

    TRY_BOOL(Kao_readMem((objectBase + g_KaoOffsets->ZONE_BOXBOUNDMIN), &(minPoint), 0x0C))
    TRY_BOOL(Kao_readMem((objectBase + g_KaoOffsets->ZONE_BOXBOUNDMAX), &(maxPoint), 0x0C))

    if NOT_NULL(callerTriMeshObject)
    {
      TRY_BOOL(
        KaoObjects_generateCubeTrimesh(
          &(localTriMeshObject),
          materialObject,
          &(minPoint),
          &(maxPoint),
          stripsPortionsObject,
          indicesArrayObject,
          texCoordsArrayObject,
          colorsArrayObject))

      (*callerTriMeshObject) = localTriMeshObject;

      /* Add object to storage (as long as the level is loaded) */
      TRY_BOOL(
        AnyArray_append(
          &(g_KaoXRayNodes_arrayTriggerMeshes),
          &(localTriMeshObject)));
    }

    if NOT_NULL(callerPropagateVisibility)
    {
      (*callerPropagateVisibility) = TRUE;
    }
  }

  return_value = TRUE;
  leave: {}
  return return_value;
}

/**
 * @brief Checking cache at the start of selected action.
 *
 * @param toggleOn Should the nodes be turned ON or OFF.
 * @param array The array containing cached `eNode` game-object base addresses.
 * @return `0` on error; `1` when cache was used; `2` when cache was empty
 * and `toogleOn` was FALSE; `3` when cache was empty and `toggleOn` was TRUE.
 */
int
KaoXRayNodes_actionStart(const BOOL toggleOn, const AnyArray * array)
{
  DWORD dwordTest;
  DWORD nodeObject;

  /* If some nodes were already found, just toggle their visibility */

  if NOT_ZERO(array->currentSize)
  {
    for (size_t i = 0; i < array->currentSize; i++)
    {
      AnyArray_getIthElement(
        array,
        &(nodeObject),
        i);

      KAO_GET_DWORD(nodeObject + g_KaoOffsets->NODE_FLAGS)

      if (toggleOn)
      {
        /* - set 0x00000001 [Node Enabled] */
        /* - set 0x20000000 [Draw Pass #2] */
        /* - clear 0x00010000 [???] */
        /* - clear 0x10000000 [Draw Pass #1] */
        dwordTest |= 0x20000001;
        dwordTest &= (~0x10010000);
      }
      else
      {
        /* - clear 0x00000001 [Node Enabled] */
        /* - clear 0x10000000 [Draw Pass #1] */
        /* - clear 0x20000000 [Draw Pass #2] */
        /* - set 0x00010000 [???] */
        dwordTest &= (~0x30000001);
        dwordTest |= 0x00010000;
      }

      KAO_PUT_DWORD((nodeObject + g_KaoOffsets->NODE_FLAGS), dwordTest);
    }

    return 1;
  }

  if IS_FALSE(toggleOn)
  {
    GUI_updateStatus(TEXT("No changes to be made... Please reload the level!"));
    return 2;
  }

  return 3;

  leave:
  {
    return 0;
  }
}


/**************************************************************/
/* Kao2 XRay Nodes :: Public functions                        */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
KaoXRayNodes_init()
{
  AnyArray_init(
    &(g_KaoXRayNodes_arrayInvisibleMeshes),
    sizeof(DWORD));

  AnyArray_init(
    &(g_KaoXRayNodes_arrayMaterials),
    sizeof(MaterialIdentifierEx));

  AnyArray_init(
    &(g_KaoXRayNodes_arrayTriggerMeshes),
    sizeof(DWORD));

  g_KaoXRayNodes_magicTextures[0] = 0;
  g_KaoXRayNodes_magicTextures[1] = 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
KaoXRayNodes_clear()
{
  AnyArray_destroy(&(g_KaoXRayNodes_arrayInvisibleMeshes));
  AnyArray_destroy(&(g_KaoXRayNodes_arrayMaterials));
  AnyArray_destroy(&(g_KaoXRayNodes_arrayTriggerMeshes));

  g_KaoXRayNodes_magicTextures[0] = 0;
  g_KaoXRayNodes_magicTextures[1] = 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
KaoXRayNodes_replaceBitmapInTexture(const DWORD idx)
{
  DWORD bitmapObject;

  if (((0 != idx) && (1 != idx)) || IS_ZERO(g_KaoXRayNodes_magicTextures[idx]))
  {
    return;
  }

  GUI_updateStatus(TEXT("Please wait... (switch focus to the game window!)"));

  TRY_BOOL(
    KaoObjects_generateBitmap(
      &(bitmapObject),
      MISC_DEFAULT_IMAGE_WIDTH,
      MISC_DEFAULT_IMAGE_WIDTH,
      g_XRayGUI_rgbaImages[idx]))

  TRY_BOOL(
    KaoObjects_replaceRefObj(
      (g_KaoXRayNodes_magicTextures[idx] + g_KaoOffsets->TEXTURE_BITMAP),
      bitmapObject))

  TRY_BOOL(KaoObjects_derefRefObj(0, bitmapObject))

  leave:
  {
    KaoMem_commAwake();
    GUI_updateStatus(TEXT("Bitmap color changed! ;)"));
  }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoXRayNodes_toggleInvisibleCollision(BOOL toggleOn)
{
  BOOL return_value = FALSE;
  BOOL usedCache = FALSE;
  DWORD dwordTest;
  TCHAR buf[BUF_SIZE];

  DWORD sceneObject;
  DWORD bitmapObject;
  DWORD textureObject;
  MaterialIdentifierEx dummyMtl;

  dwordTest = KaoXRayNodes_actionStart(
    toggleOn,
    &(g_KaoXRayNodes_arrayInvisibleMeshes));

  if EQ(0, dwordTest)
  {
    goto leave;
  }
  else if EQ(1, dwordTest)
  {
    usedCache = TRUE;
  }
  else if EQ(2, dwordTest)
  {
    return_value = TRUE;
    goto leave;
  }
  else
  {
    /* No objects cached for this level yet. */

    KAO_GET_DWORD(g_KaoOffsets->GLOBAL_GAMELET)
    KAO_GET_DWORD(dwordTest + (g_KaoOffsets->GAMELET_SCENE))
    sceneObject = dwordTest;

    /* Generate new game objects (bitmap, texture) */

    TRY_BOOL(
      KaoObjects_generateBitmap(
        &(bitmapObject),
        MISC_DEFAULT_IMAGE_WIDTH,
        MISC_DEFAULT_IMAGE_WIDTH,
        g_XRayGUI_rgbaImages[0]))

    TRY_BOOL(
      KaoObjects_generateTexture(
        &(textureObject),
        bitmapObject))

    TRY_BOOL(KaoObjects_derefRefObj(0, bitmapObject))
    g_KaoXRayNodes_magicTextures[0] = textureObject;

    /* Traverse scene, collect matching (invisible) `eTriMesh` nodes, */
    /* replace their materials */

    TRY_BOOL(
      KaoXRayNodes_invisibleCollisionTraverse(
        NULL,
        sceneObject));

    /* Dereference new objects (destroy if never used) */

    for (size_t i = 0; i < g_KaoXRayNodes_arrayMaterials.currentSize; i++)
    {
      AnyArray_getIthElement(
        &(g_KaoXRayNodes_arrayMaterials),
        &(dummyMtl),
        i);

      TRY_BOOL(
        KaoObjects_derefRefObj(0, dummyMtl.materialObject))
    }

    AnyArray_destroy(&(g_KaoXRayNodes_arrayMaterials));

    if IS_ZERO(g_KaoXRayNodes_arrayInvisibleMeshes.currentSize)
    {
      g_KaoXRayNodes_magicTextures[0] = 0;
      TRY_BOOL(KaoObjects_derefRefObj(0, textureObject))
    }
  }

  /* Show message on success */

  _sntprintf(
    buf,
    BUF_SIZE,
    TEXT("Modified %zu TriMesh nodes - %s.\nInvisible Collision Meshes toggled %s."),
    g_KaoXRayNodes_arrayInvisibleMeshes.currentSize,
    usedCache ? TEXT("used level cache") : TEXT("fresh iteration"),
    toggleOn ? TEXT("ON") : TEXT("OFF"));

  GUI_updateStatus(buf);

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoXRayNodes_toggleBoxZones(BOOL toggleOn)
{
  BOOL return_value = FALSE;
  BOOL usedCache = FALSE;
  DWORD dwordTest;
  TCHAR buf[BUF_SIZE];

  DWORD sceneObject;
  DWORD bitmapObject;
  DWORD textureObject;
  DWORD materialObject;
  DWORD stripsPortionsObject;
  DWORD indicesArrayObject;
  DWORD texCoordsArrayObject;
  DWORD colorsArrayObject;

  dwordTest = KaoXRayNodes_actionStart(
    toggleOn,
    &(g_KaoXRayNodes_arrayTriggerMeshes));

  if EQ(0, dwordTest)
  {
    goto leave;
  }
  else if EQ(1, dwordTest)
  {
    usedCache = TRUE;
  }
  else if EQ(2, dwordTest)
  {
    return_value = TRUE;
    goto leave;
  }
  else
  {
    /* No objects cached for this level yet. */

    KAO_GET_DWORD(g_KaoOffsets->GLOBAL_GAMELET)
    KAO_GET_DWORD(dwordTest + (g_KaoOffsets->GAMELET_SCENE))
    sceneObject = dwordTest;

    TRY_BOOL(
      KaoObjects_generateBitmap(
        &(bitmapObject),
        MISC_DEFAULT_IMAGE_WIDTH,
        MISC_DEFAULT_IMAGE_WIDTH,
        g_XRayGUI_rgbaImages[1]))

    TRY_BOOL(
      KaoObjects_generateTexture(
        &(textureObject),
        bitmapObject))

    TRY_BOOL(KaoObjects_derefRefObj(0, bitmapObject))
    g_KaoXRayNodes_magicTextures[1] = textureObject;

    TRY_BOOL(
      KaoObjects_generateMaterial(
        &(materialObject),
        (0x01 | 0x02),
        0x00000000,
        0x4000,
        0.5f,
        textureObject))

    TRY_BOOL(KaoObjects_derefRefObj(0, textureObject))

    TRY_BOOL(
      KaoObjects_generateCubeGeoarrays(
        &(stripsPortionsObject),
        &(indicesArrayObject),
        &(texCoordsArrayObject),
        &(colorsArrayObject)))

    /* Traverse scene, looking for any `eBoxZone` nodes, */
    /* creating them a `eTriMesh` friend */

    TRY_BOOL(
      KaoXRayNodes_boxZonesTraverse(
        NULL,
        NULL,
        sceneObject,
        materialObject,
        stripsPortionsObject,
        indicesArrayObject,
        texCoordsArrayObject,
        colorsArrayObject));

    /* Dereference new objects (destroy if never used) */

    if IS_ZERO(g_KaoXRayNodes_arrayTriggerMeshes.currentSize)
    {
      g_KaoXRayNodes_magicTextures[1] = 0;
      TRY_BOOL(KaoObjects_derefRefObj(0, materialObject))
    }

    TRY_BOOL(KaoObjects_derefRefObj(0, materialObject))
    TRY_BOOL(KaoObjects_derefRefObj(0, stripsPortionsObject))
    TRY_BOOL(KaoObjects_derefRefObj(0, indicesArrayObject))
    TRY_BOOL(KaoObjects_derefRefObj(0, texCoordsArrayObject))
    TRY_BOOL(KaoObjects_derefRefObj(0, colorsArrayObject))
  }

  /* Show message on success */

  _sntprintf(
    buf,
    BUF_SIZE,
    TEXT("Modified %zu TriMesh nodes - %s.\nBox Zones (Triggers) toggled %s."),
    g_KaoXRayNodes_arrayTriggerMeshes.currentSize,
    usedCache ? TEXT("used level cache") : TEXT("fresh iteration"),
    toggleOn ? TEXT("ON") : TEXT("OFF"));

  GUI_updateStatus(buf);

  return_value = TRUE;
  leave: {}
  return return_value;
}


/**************************************************************/

/**
 * @}
 */


/**************************************************************/
