/**************************************************************/
/* "kao_winapi_toolbase/toolbase_kao_mem.h"                   */
/**************************************************************/

#ifndef H_KAO_WINAPI_TOOLBASE_KAO_MEM
#define H_KAO_WINAPI_TOOLBASE_KAO_MEM

#include "toolbase_main.h"

/**
 * @defgroup ToolBase_Kao_Mem ToolBase :: Kao Memory
 * @{
 */


/**************************************************************/
/* Kao Memory :: Type definitions                             */
/**************************************************************/

/**
 * @brief KaoOffsets type.
 */
typedef struct kao_offsets_t KaoOffsets;

/**
 * @brief KaoOffsets structure.
 * This structure holds offsets of members of specific game-objects,
 * as well as some addresses of global game-objects and global variables.
 */
struct kao_offsets_t
{
  DWORD COMMUNICATOR_ARGS;  /**<  */

  DWORD GLOBAL_LOADING;              /**<  */
  DWORD GLOBAL_GAMELET;              /**<  */
  DWORD GLOBAL_BASICSTRING;          /**<  */
  DWORD GLOBAL_ARRAY_REF_LEVELDESC;  /**<  */
  DWORD GLOBAL_LEVELID;              /**<  */

  DWORD GAMELET_SCENE;     /**<  */
  DWORD GAMELET_HERO;      /**<  */
  DWORD GAMELET_FLOWCTRL;  /**<  */

  DWORD LEVELDESC_NAME;  /**<  */

  DWORD REFCOUNTER_COUNTER;  /**<  */

  DWORD NODE_PREVXFORM;  /**<  */
  DWORD NODE_UNUSED;     /**<  */
  DWORD NODE_PARENT;     /**<  */
  DWORD NODE_NAME;       /**<  */
  DWORD NODE_ALBOX;      /**<  */
  DWORD NODE_FLAGS;      /**<  */
  DWORD NODE_SPHERE;     /**<  */
  DWORD NODE_COLLISION;  /**<  */
  DWORD NODE_VISGROUP;   /**<  */
  DWORD NODE_VISCTRL;    /**<  */
  DWORD NODE_VISRATE;    /**<  */
  DWORD NODE_SIZE;       /**< Size of the `eNode` object */

  DWORD GROUP_NODES;  /**<  */
  DWORD GROUP_SIZE;   /**< Size of the `eGroup` object */

  DWORD ZONE_BOXBOUNDMIN;  /**<  */
  DWORD ZONE_BOXBOUNDMAX;  /**<  */

  DWORD GEOARRAY_DATA;    /**<  */
  DWORD GEOARRAY_LENGTH;  /**<  */
  DWORD GEOARRAY_SIZE;    /**< Size of the `eGeoArray` object */

  DWORD GEOMETRY_MATERIAL;     /**<  */
  DWORD GEOMETRY_BOXBOUNDMIN;  /**<  */
  DWORD GEOMETRY_BOXBOUNDMAX;  /**<  */

  DWORD TRIMESH_GEOSET;    /**<  */
  DWORD TRIMESH_MODIFIER;  /**<  */
  DWORD TRIMESH_SIZE;      /**< Size of the `eTriMesh` object */

  DWORD GEOSET_FLAGS;            /**<  */
  DWORD GEOSET_VERTEXCOUNT;      /**<  */
  DWORD GEOSET_TEXCOORDSCOUNT;   /**<  */
  DWORD GEOSET_STRIPSPORTIONS;   /**<  */
  DWORD GEOSET_INDICESARRAY;     /**<  */
  DWORD GEOSET_VERTICESARRAYS;   /**<  */
  DWORD GEOSET_NORMALSARRAYS;    /**<  */
  DWORD GEOSET_TEXCOORDSARRAYS;  /**<  */
  DWORD GEOSET_COLORSARRAY;      /**<  */
  DWORD GEOSET_TEXMAPPINGTYPES;  /**<  */
  DWORD GEOSET_DISPLAYLIST;      /**<  */
  DWORD GEOSET_CURRENTSET;       /**<  */
  DWORD GEOSET_MODIFIER;         /**<  */
  DWORD GEOSET_AABBTREE;         /**<  */
  DWORD GEOSET_SIZE;             /**< Size of the `eGeoSet` object */

  DWORD BITMAP_VW;         /**<  */
  DWORD BITMAP_VH;         /**<  */
  DWORD BITMAP_WIDTH;      /**<  */
  DWORD BITMAP_HEIGHT;     /**<  */
  DWORD BITMAP_BIND;       /**<  */
  DWORD BITMAP_PIXELS;     /**<  */
  DWORD BITMAP_PAL;        /**<  */
  DWORD BITMAP_TYPE;       /**<  */
  DWORD BITMAP_PATH;       /**<  */
  DWORD BITMAP_UNUSED;     /**<  */
  DWORD BITMAP_EVALUATOR;  /**<  */
  DWORD BITMAP_SIZE;       /**< Size of the `eBitmap` object */

  DWORD TEXTURE_FLAGS;   /**<  */
  DWORD TEXTURE_XFORM;   /**<  */
  DWORD TEXTURE_BITMAP;  /**<  */
  DWORD TEXTURE_UNUSED;  /**<  */
  DWORD TEXTURE_SIZE;    /**< Size of the `eTexture` object */

  DWORD MATERIAL_TEXTURES;     /**<  */
  DWORD MATERIAL_FLAGS;        /**<  */
  DWORD MATERIAL_STATE;        /**<  */
  DWORD MATERIAL_COLLISION;    /**<  */
  DWORD MATERIAL_UNUSED;       /**<  */
  DWORD MATERIAL_SOUND;        /**<  */
  DWORD MATERIAL_NAME;         /**<  */
  DWORD MATERIAL_TRANSPLAYER;  /**<  */
  DWORD MATERIAL_ALPHAREF;     /**<  */
  DWORD MATERIAL_SIZE;         /**< Size of the `eMaterial` object */

  DWORD IC_OBJECT;    /**< `eObject` instance counter */
  DWORD IC_NODE;      /**< `eNode` instance counter */
  DWORD IC_GROUP;     /**< `eGroup` instance counter */
  DWORD IC_XFORM;     /**< `eTransform` instance counter */
  DWORD IC_TRIMESH;   /**< `eTriMesh` instance counter */
  DWORD IC_GEOSET;    /**< `eGeoSet` instance counter */
  DWORD IC_BITMAP;    /**< `eBitmap` instance counter */
  DWORD IC_TEXTURE;   /**< `eTexture` instance counter */
  DWORD IC_MATERIAL;  /**< `eMaterial` instance counter */

  DWORD VPTR_BITMAP;           /**< `eBitmap` virtual functions table */
  DWORD VPTR_TEXTURE;          /**< `eTexture` virtual functions table */
  DWORD VPTR_MATERIAL;         /**< `eMaterial` virtual functions table */
  DWORD VPTR_NODE;             /**< `eNode` virtual functions table */
  DWORD VPTR_BOXZONE;          /**< `eBoxZone` virtual functions table */
  DWORD VPTR_GROUP;            /**< `eGroup` virtual functions table */
  DWORD VPTR_ENVIRONMENT;      /**< `eEnvironment` virtual functions table */
  DWORD VPTR_TRANSFORM;        /**< `eTransform` virtual functions table */
  DWORD VPTR_BILLBOARD;        /**< `eBillboard` virtual functions table */
  DWORD VPTR_PROXY;            /**< `eProxy` virtual functions table */
  DWORD VPTR_PIVOT;            /**< `ePivot` virtual functions table */
  DWORD VPTR_ACTOR;            /**< `Actor` virtual functions table */
  DWORD VPTR_WSGROUP;          /**< `eWSGroup` virtual functions table */
  DWORD VPTR_SCENE;            /**< `eScene` virtual functions table */
  DWORD VPTR_TRIMESH;          /**< `eTriMesh` virtual functions table */
  DWORD VPTR_GEOSET;           /**< `eGeoSet` virtual functions table */
  DWORD VPTR_GEOARRAY_USHORT;  /**< `eGeoArray<ushort>` virtual functions table */
  DWORD VPTR_GEOARRAY_POINT2;  /**< `eGeoArray<ePoint2>` virtual functions table */
  DWORD VPTR_GEOARRAY_POINT4;  /**< `eGeoArray<ePoint4>` virtual functions table */
  DWORD VPTR_3FXARRAY;         /**< `e3fXArray` virtual functions table */

  DWORD METHOD_MALLOC;  /**< In-game `malloc()` wrapper method */
  DWORD METHOD_MFREE;   /**< In-game `free()` wrapper method */
};


/**************************************************************/
/* Kao Memory :: Public data                                  */
/**************************************************************/

/**
 * @brief Offsets compatible with "Kangurek Kao: Runda 2" (retail version).
 */
extern KaoOffsets KAO2PL_OFFSETS;

/**
 * @brief Offsets compatible with "Kao: Tajemnica Wulkanu".
 */
extern KaoOffsets KAO3PL_OFFSETS;

/**
 * @brief Pointer to the offsets structure selected for current game version.
 */
extern KaoOffsets * g_KaoOffsets;


/**************************************************************/
/* Kao Memory :: Public functions                             */
/**************************************************************/

/**
 * @brief Initializing global data (static offset structures).
 */
VOID KaoMem_init();

/**
 * @brief Awake the game after series of `KAOHACK_COMM` cals.
 *
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoMem_commAwake();

/**
 * @brief Remotely allocates memory block for the game process.
 *
 * Uses "Kao Hacks Communicator" to call `malloc(size)`.
 * Afterwards the game hangs in an infinite loop until awaken.
 * @param[out] address Where the result of `malloc()`, a 32-bit pointer
 * in game memory space, will be stored.
 * @param[in] size The size to be passed to `malloc()` C-library function.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoMem_remoteMalloc(LPDWORD address, const DWORD size);

/**
 * @brief Remotely frees memory block in the game process.
 *
 * Uses "Kao Hacks Communicator" to call `free(block)`.
 * Afterwards the game hangs in an infinite loop until awaken.
 * @param address 32-bit pointer obtained via `malloc()`,
 * that will be passed to `free()` C-library function.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoMem_remoteMfree(const DWORD address);

/**
 * @brief Reads contents of an "eString" game-object into unicode char buffer.
 *
 * @param[out] unicodeResult Where the text should be stored.
 * @param stringWrapperAddress Address of the "eString" wrapper game-object.
 * @param isUnicode TRUE if "eString" points to Unicode char buffer, otherwise
 * treat the sourcse as "ANSI Windows-1250".
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoMem_readString(
  WCHAR unicodeResult[MEDIUM_BUF_SIZE],
  const DWORD stringWrapperAddress,
  const BOOL isUnicode);

/**
 * @brief Reads selected level's name into unicode char buffer.
 *
 * @param[out] unicodeResult Where the text should be stored.
 * @param levelId Zero-based level index.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoMem_readLevelName(
  WCHAR unicodeResult[MEDIUM_BUF_SIZE],
  const DWORD levelId);


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO_WINAPI_TOOLBASE_KAO_MEM */


/**************************************************************/
