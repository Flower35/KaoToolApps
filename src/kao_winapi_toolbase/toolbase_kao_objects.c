/**************************************************************/
/* "kao_winapi_toolbase/toolbase_kao_objects.c"               */
/**************************************************************/

#include "toolbase_kao_objects.h"

#include "toolbase_kao_hacks.h"
#include "toolbase_kao_mem.h"
#include "toolbase_kao.h"

/**
 * @addtogroup ToolBase_Kao_Objects
 * @{
 */


/**************************************************************/
/* Kao Objects :: Public functions                            */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_putEmptyBasicString(const DWORD address)
{
  BOOL return_value = FALSE;
  DWORD dwordTest;

  KAO_PUT_DWORD(address, g_KaoOffsets->GLOBAL_BASICSTRING);

  KAO_INC_DWORD(g_KaoOffsets->GLOBAL_BASICSTRING);

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_derefRefObj(const DWORD address, DWORD objectBase)
{
  BOOL return_value = FALSE;
  DWORD dwordTest;
  DWORD objectDtorMethod;

  if NOT_ZERO(address)
  {
    KAO_GET_DWORD(address)
    objectBase = dwordTest;
  }

  if NOT_ZERO(objectBase)
  {
    KAO_GET_DWORD(objectBase + g_KaoOffsets->REFCOUNTER_COUNTER)
    dwordTest -= 1;

    if IS_ZERO(dwordTest)
    {
      KAO_GET_DWORD(objectBase)
      KAO_GET_DWORD(dwordTest + 0x0C)
      objectDtorMethod = dwordTest;

      KAO_PUT_DWORD((g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 1), objectDtorMethod)
      KAO_PUT_DWORD((g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 2), objectBase)
      KAO_PUT_DWORD((g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 3), 0x01)

      KAO_PUT_DWORD(
        (g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 0),
        ((1 << 3) | KAOHACK_COMM_THISCALL))

      KAO_WAIT_DWORD_NE((g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 0), KAOHACK_COMM_WAITING)

      if NOT_ZERO(address)
      {
        KAO_PUT_DWORD(address, 0)
      }
    }
    else
    {
      KAO_PUT_DWORD((objectBase + g_KaoOffsets->REFCOUNTER_COUNTER), dwordTest)
    }
  }

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_putRefObj(const DWORD address, const DWORD objectBase)
{
  BOOL return_value = FALSE;
  DWORD dwordTest;

  KAO_PUT_DWORD(address, objectBase)

  if NOT_ZERO(objectBase)
  {
    KAO_INC_DWORD(objectBase + g_KaoOffsets->REFCOUNTER_COUNTER)
  }

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_replaceRefObj(const DWORD address, const DWORD objectBase)
{
  BOOL return_value = FALSE;

  TRY_BOOL(KaoObjects_derefRefObj(address, 0))
  TRY_BOOL(KaoObjects_putRefObj(address, objectBase))

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_attachNode(const DWORD groupObject, const DWORD childNode)
{
  BOOL return_value = FALSE;
  DWORD dwordTest;
  DWORD groupAttachMethod;

  KAO_GET_DWORD(groupObject);
  KAO_GET_DWORD(dwordTest + 0x0070);
  groupAttachMethod = dwordTest;

  KAO_PUT_DWORD((g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 1), groupAttachMethod)
  KAO_PUT_DWORD((g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 2), groupObject)
  KAO_PUT_DWORD((g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 3), childNode)
  KAO_PUT_DWORD((g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 4), 0x00)

  KAO_PUT_DWORD(
    (g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 0),
    ((2 << 3) | KAOHACK_COMM_THISCALL))

  KAO_WAIT_DWORD_NE((g_KaoOffsets->COMMUNICATOR_ARGS + 4 * 0), KAOHACK_COMM_WAITING)

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_generateBitmap(
  LPDWORD bitmapResult,
  const DWORD width,
  const DWORD height,
  const LPDWORD sourcePixels)
{
  BOOL return_value = FALSE;

  DWORD dwordTest;

  DWORD bitmapBase;
  DWORD bitmapPixels;

  /* Remote memory allocations */

  const DWORD pixelsSize = 0x04 * width * height;

  TRY_BOOL(KaoMem_remoteMalloc(&(bitmapPixels), pixelsSize))
  TRY_BOOL(KaoMem_remoteMalloc(&(bitmapBase), g_KaoOffsets->BITMAP_SIZE))

  /* Generating "eBitmap" object */

  TRY_BOOL(Kao_writeMem(bitmapPixels, sourcePixels, pixelsSize))

  KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)
  KAO_INC_DWORD(g_KaoOffsets->IC_BITMAP)

  KAO_PUT_DWORD(bitmapBase, g_KaoOffsets->VPTR_BITMAP)
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->BITMAP_VW), width)
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->BITMAP_VH), height)
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->BITMAP_WIDTH), width)
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->BITMAP_HEIGHT), height)
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->BITMAP_BIND), 0)
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->BITMAP_PIXELS), bitmapPixels)
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->BITMAP_PAL), 0)
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->BITMAP_TYPE), 0)
  TRY_BOOL(KaoObjects_putEmptyBasicString(bitmapBase + g_KaoOffsets->BITMAP_PATH))
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->BITMAP_UNUSED), 0)
  KAO_PUT_DWORD((bitmapBase + g_KaoOffsets->BITMAP_EVALUATOR), 0)

  /* Done */

  (*bitmapResult) = bitmapBase;

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_generateTexture(
  LPDWORD textureResult,
  const DWORD bitmapObject)
{
  BOOL return_value = FALSE;
  DWORD dwordTest;

  DWORD textureBase;

  /* Remote memory allocations */

  TRY_BOOL(KaoMem_remoteMalloc(&(textureBase), g_KaoOffsets->TEXTURE_SIZE))

  /* Generating "eTexture" object */

  KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)
  KAO_INC_DWORD(g_KaoOffsets->IC_TEXTURE)

  KAO_PUT_DWORD(textureBase, g_KaoOffsets->VPTR_TEXTURE)
  KAO_PUT_DWORD((textureBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
  KAO_PUT_DWORD((textureBase + g_KaoOffsets->TEXTURE_FLAGS), 0x04)
  KAO_PUT_DWORD((textureBase + g_KaoOffsets->TEXTURE_XFORM), 0)
  TRY_BOOL(KaoObjects_putRefObj((textureBase + g_KaoOffsets->TEXTURE_BITMAP), bitmapObject))
  KAO_PUT_DWORD((textureBase + g_KaoOffsets->TEXTURE_UNUSED), 0)

  /* Done */

  (*textureResult) = textureBase;

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_generateMaterial(
  LPDWORD materialResult,
  WORD materialFlags,
  DWORD materialCollision,
  WORD materialSound,
  FLOAT materialAlphaRef,
  const DWORD textureObject)
{
  BOOL return_value = FALSE;
  WORD wordTest;
  DWORD dwordTest;

  DWORD materialBase;
  DWORD materialTexturesArray;

  /* Remote memory allocations */

  TRY_BOOL(KaoMem_remoteMalloc(&(materialTexturesArray), 0x04))
  TRY_BOOL(KaoMem_remoteMalloc(&(materialBase), g_KaoOffsets->MATERIAL_SIZE))

  /* Generating "eMaterial" object */

  TRY_BOOL(KaoObjects_putRefObj(materialTexturesArray, textureObject))

  KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)
  KAO_INC_DWORD(g_KaoOffsets->IC_MATERIAL)

  KAO_PUT_DWORD(materialBase, g_KaoOffsets->VPTR_MATERIAL)
  KAO_PUT_DWORD((materialBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
  KAO_PUT_DWORD((materialBase + g_KaoOffsets->MATERIAL_TEXTURES + 0x00), 0x01)
  KAO_PUT_DWORD((materialBase + g_KaoOffsets->MATERIAL_TEXTURES + 0x04), 0x01)
  KAO_PUT_DWORD((materialBase + g_KaoOffsets->MATERIAL_TEXTURES + 0x08), materialTexturesArray)
  KAO_PUT_WORD((materialBase + g_KaoOffsets->MATERIAL_FLAGS),materialFlags)
  KAO_PUT_DWORD((materialBase + g_KaoOffsets->MATERIAL_STATE), 0)
  KAO_PUT_DWORD((materialBase + g_KaoOffsets->MATERIAL_COLLISION), materialCollision)
  KAO_PUT_WORD((materialBase + g_KaoOffsets->MATERIAL_UNUSED), 0)
  KAO_PUT_WORD((materialBase + g_KaoOffsets->MATERIAL_SOUND), materialSound)
  TRY_BOOL(KaoObjects_putEmptyBasicString(materialBase + g_KaoOffsets->MATERIAL_NAME))
  KAO_PUT_DWORD((materialBase + g_KaoOffsets->MATERIAL_TRANSPLAYER), 0)
  TRY_BOOL(Kao_writeMem((materialBase + g_KaoOffsets->MATERIAL_ALPHAREF), &(materialAlphaRef), 0x04))

  /* Done */

  (*materialResult) = materialBase;

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_constructNode(const DWORD nodeBase)
{
  BOOL return_value = FALSE;
  WORD wordTest;
  DWORD dwordTest;

  KAO_PUT_DWORD(nodeBase, g_KaoOffsets->VPTR_NODE)
  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_PREVXFORM), 0)
  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_UNUSED), 0x00FFFFFF)
  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_PARENT), 0)
  TRY_BOOL(KaoObjects_putEmptyBasicString(nodeBase + g_KaoOffsets->NODE_NAME))
  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_ALBOX), 0)
  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_FLAGS), 0x249D)

  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_SPHERE + 0x00), 0x00000000)
  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_SPHERE + 0x04), 0x00000000)
  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_SPHERE + 0x08), 0x00000000)
  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_SPHERE + 0x0C), 0xBF800000)

  KAO_PUT_WORD((nodeBase + g_KaoOffsets->NODE_COLLISION), 0x00FF)

  if NOT_ZERO(g_KaoOffsets->NODE_VISGROUP)
  {
    KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_VISGROUP), 0xFFFFFFFF)
  }

  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_VISCTRL), 0)
  KAO_PUT_DWORD((nodeBase + g_KaoOffsets->NODE_VISRATE), 0x3F800000)

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_constructGeometry(
  const DWORD geometryBase,
  const DWORD materialObject,
  const ePoint3 * minPoint,
  const ePoint3 * maxPoint)
{
  BOOL return_value = FALSE;

  TRY_BOOL(KaoObjects_constructNode(geometryBase))

  TRY_BOOL(KaoObjects_putRefObj((geometryBase + g_KaoOffsets->GEOMETRY_MATERIAL), materialObject))
  TRY_BOOL(Kao_writeMem((geometryBase + g_KaoOffsets->GEOMETRY_BOXBOUNDMIN), minPoint, 0x0C))
  TRY_BOOL(Kao_writeMem((geometryBase + g_KaoOffsets->GEOMETRY_BOXBOUNDMAX), maxPoint, 0x0C))

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_generateCubeGeoarrays(
  LPDWORD stripsPortionsResult,
  LPDWORD indicesArrayResult,
  LPDWORD texCoordsArrayResult,
  LPDWORD colorsArrayResult)
{
  BOOL return_value = FALSE;
  WORD wordTest;
  DWORD i;
  DWORD dwordTest;
  ePoint4 dummyPoint;

  DWORD stripsPortionsBase;
  DWORD stripsPortionsData;

  DWORD indicesArrayBase;
  DWORD indicesArrayData;

  DWORD texCoordsArrayBase;
  DWORD texCoordsArrayData;

  DWORD colorsArrayBase;
  DWORD colorsArrayData;

  const DWORD numTriangleStrips = 1;
  const DWORD indicesPerStrip = 14;
  const DWORD numVertices = 8;

  const USHORT triangulatedCubeIndices[14] =
  {
    0, 1, 2, 3, 7, 1, 5, 0, 4, 2, 6, 7, 4, 5
  };

  const FLOAT textureCoords[2 * 8] =
  {
    /* [0] */ 0.0f, 0.0f,
    /* [1] */ 0.0f, 1.0f,
    /* [2] */ 1.0f, 0.0f,
    /* [3] */ 1.0f, 1.0f,
    /* [4] */ 1.0f, 0.0f,
    /* [5] */ 1.0f, 1.0f,
    /* [6] */ 0.0f, 0.0f,
    /* [7] */ 0.0f, 1.0f
  };

  /* Remote memory allocations */

  TRY_BOOL(KaoMem_remoteMalloc(&(stripsPortionsBase), g_KaoOffsets->GEOARRAY_SIZE))
  TRY_BOOL(KaoMem_remoteMalloc(&(stripsPortionsData), (0x02 * numTriangleStrips)))

  TRY_BOOL(KaoMem_remoteMalloc(&(indicesArrayBase), g_KaoOffsets->GEOARRAY_SIZE))
  TRY_BOOL(KaoMem_remoteMalloc(&(indicesArrayData), (0x02 * indicesPerStrip)))

  TRY_BOOL(KaoMem_remoteMalloc(&(texCoordsArrayBase), g_KaoOffsets->GEOARRAY_SIZE))
  TRY_BOOL(KaoMem_remoteMalloc(&(texCoordsArrayData), (0x08 * numVertices)))

  TRY_BOOL(KaoMem_remoteMalloc(&(colorsArrayBase), g_KaoOffsets->GEOARRAY_SIZE))
  TRY_BOOL(KaoMem_remoteMalloc(&(colorsArrayData), (0x10 * numVertices)))

  /* Generating "stripsPortions" (eGeoArray<ushort>) object */

  KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)
  KAO_PUT_DWORD(stripsPortionsBase, g_KaoOffsets->VPTR_GEOARRAY_USHORT)
  KAO_PUT_DWORD((stripsPortionsBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
  KAO_PUT_DWORD((stripsPortionsBase + g_KaoOffsets->GEOARRAY_DATA), stripsPortionsData)
  KAO_PUT_DWORD((stripsPortionsBase + g_KaoOffsets->GEOARRAY_LENGTH), numTriangleStrips)

  KAO_PUT_WORD(stripsPortionsData, indicesPerStrip)

  /* Generating "indicesArray" (eGeoArray<ushort>) object */

  KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)
  KAO_PUT_DWORD(indicesArrayBase, g_KaoOffsets->VPTR_GEOARRAY_USHORT)
  KAO_PUT_DWORD((indicesArrayBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
  KAO_PUT_DWORD((indicesArrayBase + g_KaoOffsets->GEOARRAY_DATA), indicesArrayData)
  KAO_PUT_DWORD((indicesArrayBase + g_KaoOffsets->GEOARRAY_LENGTH), indicesPerStrip)

  TRY_BOOL(Kao_writeMem(indicesArrayData, triangulatedCubeIndices, (0x02 * indicesPerStrip)))

  /* Generating "texCoordsArray" (eGeoArray<ePoint2>) object */

  KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)
  KAO_PUT_DWORD(texCoordsArrayBase, g_KaoOffsets->VPTR_GEOARRAY_POINT2)
  KAO_PUT_DWORD((texCoordsArrayBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
  KAO_PUT_DWORD((texCoordsArrayBase + g_KaoOffsets->GEOARRAY_DATA), texCoordsArrayData)
  KAO_PUT_DWORD((texCoordsArrayBase + g_KaoOffsets->GEOARRAY_LENGTH), numVertices)

  TRY_BOOL(Kao_writeMem(texCoordsArrayData, textureCoords, (0x08 * numVertices)))

  /* Generating "colorsArray" (eGeoArray<ePoint4>) object */

  KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)
  KAO_PUT_DWORD(colorsArrayBase, g_KaoOffsets->VPTR_GEOARRAY_POINT4)
  KAO_PUT_DWORD((colorsArrayBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
  KAO_PUT_DWORD((colorsArrayBase + g_KaoOffsets->GEOARRAY_DATA), colorsArrayData)
  KAO_PUT_DWORD((colorsArrayBase + g_KaoOffsets->GEOARRAY_LENGTH), numVertices)

  dummyPoint.x = 1.0f;
  dummyPoint.y = 1.0f;
  dummyPoint.z = 1.0f;
  dummyPoint.w = 1.0f;

  for (i = 0; i < numVertices; i++)
  {
    TRY_BOOL(Kao_writeMem((colorsArrayData + 0x10 * i), &(dummyPoint), 0x10))
  }

  /* Done */

  (*stripsPortionsResult) = stripsPortionsBase;
  (*indicesArrayResult)   = indicesArrayBase;
  (*texCoordsArrayResult) = texCoordsArrayBase;
  (*colorsArrayResult)    = colorsArrayBase;

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_generateCubeTrimesh(
  LPDWORD triMeshResult,
  const DWORD materialObject,
  const ePoint3 * minPoint,
  const ePoint3 * maxPoint,
  const DWORD stripsPortionsObject,
  const DWORD indicesArrayObject,
  const DWORD texCoordsArrayObject,
  const DWORD colorsArrayObject)
{
  BOOL return_value = FALSE;
  DWORD dwordTest;
  ePoint4 dummyPoint;

  DWORD triMeshBase;
  DWORD geoSetBase;

  DWORD verticesBase;
  DWORD verticesData;

  const DWORD numVertices = 8;

  /* Remote memory allocations */

  TRY_BOOL(KaoMem_remoteMalloc(&(triMeshBase), g_KaoOffsets->TRIMESH_SIZE))

  TRY_BOOL(KaoMem_remoteMalloc(&(geoSetBase), g_KaoOffsets->GEOSET_SIZE))

  /* (...) kao_tw */
  for (DWORD i = 0; i < (g_KaoOffsets->GEOSET_SIZE / 4); i++)
  {
    KAO_PUT_DWORD((geoSetBase + 0x04 * i), 0)
  }

  TRY_BOOL(KaoMem_remoteMalloc(&(verticesBase), g_KaoOffsets->GEOARRAY_SIZE))
  TRY_BOOL(KaoMem_remoteMalloc(&(verticesData), (0x10 * numVertices)))

  /* Generating "eTriMesh" object */

  KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)
  KAO_INC_DWORD(g_KaoOffsets->IC_NODE)
  KAO_INC_DWORD(g_KaoOffsets->IC_TRIMESH)

  TRY_BOOL(KaoObjects_constructGeometry(triMeshBase, materialObject, minPoint, maxPoint))
  KAO_PUT_DWORD(triMeshBase, g_KaoOffsets->VPTR_TRIMESH)
  KAO_PUT_DWORD((triMeshBase + g_KaoOffsets->NODE_FLAGS), 0x2000209D)
  KAO_PUT_DWORD((triMeshBase + g_KaoOffsets->TRIMESH_GEOSET), geoSetBase)

  if NOT_ZERO(g_KaoOffsets->TRIMESH_MODIFIER)
  {
    KAO_PUT_DWORD((triMeshBase + g_KaoOffsets->TRIMESH_MODIFIER), 0)
  }

  /* Generating "vertices" (eGeoArray<ePoint4>) object */

  KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)

  KAO_PUT_DWORD(verticesBase, g_KaoOffsets->VPTR_3FXARRAY)
  KAO_PUT_DWORD((verticesBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
  KAO_PUT_DWORD((verticesBase + g_KaoOffsets->GEOARRAY_DATA), verticesData)
  KAO_PUT_DWORD((verticesBase + g_KaoOffsets->GEOARRAY_LENGTH), numVertices)

  dummyPoint.w = 1.0f;

  /* [0] front, bottom-left */
  dummyPoint.x = minPoint->x;
  dummyPoint.y = minPoint->y;
  dummyPoint.z = minPoint->z;
  TRY_BOOL(Kao_writeMem((verticesData + 0x10 * 0), &(dummyPoint), 0x10))

  /* [1] front, top-left */
  dummyPoint.z = maxPoint->z;
  TRY_BOOL(Kao_writeMem((verticesData + 0x10 * 1), &(dummyPoint), 0x10))

  /* [2] front, bottom-right */
  dummyPoint.x = maxPoint->x;
  dummyPoint.z = minPoint->z;
  TRY_BOOL(Kao_writeMem((verticesData + 0x10 * 2), &(dummyPoint), 0x10))

  /* [3] front, top-right */
  dummyPoint.z = maxPoint->z;
  TRY_BOOL(Kao_writeMem((verticesData + 0x10 * 3), &(dummyPoint), 0x10))

  /* [4] back, bottom-left */
  dummyPoint.x = minPoint->x;
  dummyPoint.y = maxPoint->y;
  dummyPoint.z = minPoint->z;
  TRY_BOOL(Kao_writeMem((verticesData + 0x10 * 4), &(dummyPoint), 0x10))

  /* [5] back, top-left */
  dummyPoint.z = maxPoint->z;
  TRY_BOOL(Kao_writeMem((verticesData + 0x10 * 5), &(dummyPoint), 0x10))

  /* [6] back, bottom-right */
  dummyPoint.x = maxPoint->x;
  dummyPoint.z = minPoint->z;
  TRY_BOOL(Kao_writeMem((verticesData + 0x10 * 6), &(dummyPoint), 0x10))

  /* [7] back, top-right */
  dummyPoint.z = maxPoint->z;
  TRY_BOOL(Kao_writeMem((verticesData + 0x10 * 7), &(dummyPoint), 0x10))

  /* Generating "eGeoSet" object */

  KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)
  KAO_INC_DWORD(g_KaoOffsets->IC_GEOSET)
  KAO_PUT_DWORD(geoSetBase, g_KaoOffsets->VPTR_GEOSET)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_FLAGS), 0x0F)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_VERTEXCOUNT), numVertices)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_TEXCOORDSCOUNT), 0x01)
  TRY_BOOL(KaoObjects_putRefObj((geoSetBase + g_KaoOffsets->GEOSET_STRIPSPORTIONS), stripsPortionsObject))
  TRY_BOOL(KaoObjects_putRefObj((geoSetBase + g_KaoOffsets->GEOSET_INDICESARRAY), indicesArrayObject))
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_VERTICESARRAYS + 0x00), verticesBase)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_VERTICESARRAYS + 0x04), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_NORMALSARRAYS + 0x00), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_NORMALSARRAYS + 0x04), 0)
  TRY_BOOL(KaoObjects_putRefObj((geoSetBase + g_KaoOffsets->GEOSET_TEXCOORDSARRAYS), texCoordsArrayObject))
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_TEXCOORDSARRAYS + 0x04), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_TEXCOORDSARRAYS + 0x08), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_TEXCOORDSARRAYS + 0x0C), 0)
  TRY_BOOL(KaoObjects_putRefObj((geoSetBase + g_KaoOffsets->GEOSET_COLORSARRAY), colorsArrayObject))
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_TEXMAPPINGTYPES + 0x00), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_TEXMAPPINGTYPES + 0x04), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_TEXMAPPINGTYPES + 0x08), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_TEXMAPPINGTYPES + 0x0C), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_DISPLAYLIST), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_CURRENTSET), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_MODIFIER), 0)
  KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_AABBTREE), 0)

  /* Done */

  (*triMeshResult) = triMeshBase;

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_resetTrimeshVertexColors(const DWORD triMeshBase)
{
  BOOL return_value = FALSE;
  DWORD i;
  DWORD dwordTest;

  const ePoint4 dummyColor =
  {
    .x = 1.f, .y = 1.f, .z = 1.f, .w = 1.f
  };

  DWORD geoSetBase;
  DWORD colorsBase;
  DWORD colorsData;
  DWORD numVertices;

  /* Find "eGeoArray<ePoint4>" reference */

  KAO_GET_DWORD(triMeshBase + g_KaoOffsets->TRIMESH_GEOSET);
  geoSetBase = dwordTest;

  if IS_ZERO(geoSetBase)
  {
    return TRUE;
  }

  KAO_GET_DWORD(geoSetBase + g_KaoOffsets->GEOSET_COLORSARRAY);
  colorsBase = dwordTest;

  if IS_ZERO(colorsBase)
  {
    KAO_GET_DWORD(geoSetBase + g_KaoOffsets->GEOSET_VERTEXCOUNT);
    numVertices = dwordTest;

    TRY_BOOL(KaoMem_remoteMalloc(&(colorsBase), g_KaoOffsets->GEOARRAY_SIZE));
    TRY_BOOL(KaoMem_remoteMalloc(&(colorsData), (0x10 * numVertices)))

    KAO_INC_DWORD(g_KaoOffsets->IC_OBJECT)

    KAO_PUT_DWORD(colorsBase, g_KaoOffsets->VPTR_GEOARRAY_POINT4)
    KAO_PUT_DWORD((colorsBase + g_KaoOffsets->REFCOUNTER_COUNTER), 0x01)
    KAO_PUT_DWORD((colorsBase + g_KaoOffsets->GEOARRAY_DATA), colorsData)
    KAO_PUT_DWORD((colorsBase + g_KaoOffsets->GEOARRAY_LENGTH), numVertices)

    KAO_PUT_DWORD((geoSetBase + g_KaoOffsets->GEOSET_COLORSARRAY), colorsBase);
  }
  else
  {
    KAO_GET_DWORD(colorsBase + g_KaoOffsets->GEOARRAY_DATA);
    colorsData = dwordTest;

    KAO_GET_DWORD(colorsBase + g_KaoOffsets->GEOARRAY_LENGTH);
    numVertices = dwordTest;
  }

  /* Replace all vertex colors */

  for (i = 0; i < numVertices; i++)
  {
    TRY_BOOL(Kao_writeMem((colorsData + 0x10 * i), &(dummyColor), 0x10))
  }

  /* Done */

  return_value = TRUE;
  leave: {}
  return return_value;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
KaoObjects_assignCompatibleMaterial(
  const DWORD triMeshBase,
  AnyArray * materialStorage,
  const DWORD textureObject)
{
  BOOL return_value = FALSE;
  DWORD dwordTest;
  WORD wordTest;
  INT i;
  MaterialIdentifierEx dummyMtl;

  memset(&(dummyMtl), 0x00, sizeof(MaterialIdentifierEx));

  KAO_GET_DWORD(triMeshBase + g_KaoOffsets->GEOMETRY_MATERIAL);
  dummyMtl.materialObject = dwordTest;
  if IS_ZERO(dummyMtl.materialObject)
  {
    return TRUE;
  }

  KAO_GET_DWORD(dummyMtl.materialObject + g_KaoOffsets->MATERIAL_COLLISION);
  dummyMtl.identifiers.collisionType = dwordTest;

  KAO_GET_WORD(dummyMtl.materialObject + g_KaoOffsets->MATERIAL_SOUND);
  dummyMtl.identifiers.soundType = wordTest;

  i = AnyArray_partialMatch(
    materialStorage,
    &(dummyMtl),
    0,
    sizeof(dummyMtl.identifiers));

  if (i >= 0)
  {
    AnyArray_getIthElement(materialStorage, &(dummyMtl), i);
  }
  else
  {
    TRY_BOOL(
      KaoObjects_generateMaterial(
        &(dummyMtl.materialObject),
        (0x01 | 0x02),
        dummyMtl.identifiers.collisionType,
        dummyMtl.identifiers.soundType,
        0.5f,
        textureObject))

    TRY_BOOL(AnyArray_append(materialStorage, &(dummyMtl)));
  }

  TRY_BOOL(
    KaoObjects_replaceRefObj(
      (triMeshBase + g_KaoOffsets->GEOMETRY_MATERIAL),
      dummyMtl.materialObject))

  /* Done */

  return_value = TRUE;
  leave: {}
  return return_value;
}


/**************************************************************/

/**
 * @}
 */


/**************************************************************/
