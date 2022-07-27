/**************************************************************/
/* "kao_winapi_toolbase/toolbase_kao_objects.h"               */
/**************************************************************/

#ifndef H_KAO_WINAPI_TOOLBASE_KAO_OBJECTS
#define H_KAO_WINAPI_TOOLBASE_KAO_OBJECTS

#include "toolbase_main.h"

#include "toolbase_anyarray.h"

/**
 * @defgroup ToolBase_Kao_Objects ToolBase :: Kao Objects
 * @{
 */


/**************************************************************/
/* Kao Objects :: Type definitions                            */
/**************************************************************/

/**
 * @brief MaterialIdentifier type.
 */
typedef struct material_identifier_t MaterialIdentifier;

/**
 * @brief MaterialIdentifier structure.
 * Stores material collision type and and material sound type.
 */
struct material_identifier_t
{
  DWORD collisionType;  /**< `eMaterial` Collision Type */
  WORD soundType;       /**< `eMaterial` Sound Type */
};

/**
 * @brief MaterialIdentifierEx type.
 */
typedef struct material_identifier_ex_t MaterialIdentifierEx;

/**
 * @brief MaterialIdentifierEx structure.
 * Identifies material by its collision and sound properties.
 */
struct material_identifier_ex_t
{
  MaterialIdentifier identifiers;  /**< `eMaterial` identification */
  DWORD materialObject;            /**< Generated `eMaterial` game object */
};


/**************************************************************/
/* Kao Objects :: Public functions                            */
/**************************************************************/

/**
 * @brief Initializes `eString` member of a game-object.
 *
 * Copies the `GLOBAL_BASICSTRING` object address (thus creating `eString`
 * wrapper) into destination address and increases its reference counter.
 * @param address The destination address (32-bit pointer in game
 * memory space) of the `eString` member.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_putEmptyBasicString(const DWORD address);

/**
 * @brief Dereferences `eRefCounter`-derived game-object.
 *
 * Decreases game-object's reference counter. If the counter reaches 0,
 * game-object's virtual destructor is called remotely, and then
 * the game hangs in an infinite loop until awaken.
 * @param address Address of a 32-bit pointer (game-object reference).
 * Used to safely remove a link to `eRefCounter`-derived game-object stored
 * within other game-object or stored in game's global address space.
 * @param objectBase Base address of the `eRefCounter`-derived game-object
 * (32-bit pointer in game memory space). Ignored if `address` is not 0.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_derefRefObj(const DWORD address, DWORD objectBase);

/**
 * @brief Copies `eRefCounter`-derived game-object.
 *
 * Puts `objectBase` pointer at `address`. If `objectBase` is not 0,
 * its reference counter is increased.
 * @param address Destination address in game memory space, where the
 * 32-bit pointer `objectBase` will be stored.
 * @param objectBase Base address of the `eRefCounter`-derived game-object
 * (32-bit pointer in game memory space).
 * Can be set to 0 (useful when constructing new game-objects).
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_putRefObj(const DWORD address, const DWORD objectBase);

/**
 * @brief Replaces `eRefCounter`-derived game-object.
 *
 * This function uses `KaoObects_derefRefObj` and `KaoObject_putRedObj`
 * functions in order to remove old game-object link and replace it
 * with a new game-object link.
 * @param address Destination address in game memory space, where the
 * 32-bit pointer `objectBase` will be stored.
 * @param objectBase Base address of the `eRefCounter`-derived game-object
 * (32-bit pointer in game memory space).
 * Can be set to 0 (useful when constructing new game-objects).
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_replaceRefObj(const DWORD address, const DWORD objectBase);

/**
 * @brief Appends `eNode` game-object into the list in `eGroup` game-object.
 *
 * This function remotely calls a virtual method of `groupObject`
 * in order to attach some `childeNode` into that group.
 * Afterwards the game hangs in an infinite loop until awaken.
 * @param groupObject Base address of the `eGroup`-derived game object
 * (destination group). Must not be zero.
 * @param childNode Base address of the `eNode`-derived game-object
 * (source node). Must not be zero.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_attachNode(const DWORD groupObject, const DWORD childNode);

/**
 * @brief Generates `eBitmap` game-object from a source.
 *
 * Remotely allocates memory and constructs `eBitmap` game-object
 * from given parameters (source pixels, width, height).
 * The new object will have its reference counter set to 1.
 * Afterwards the game hangs in an infinite loop until awaken.
 * @param[out] bitmapResult Where the new `eBitmap`
 * game-object's base address will be stored.
 * @param width Image width in pixels.
 * @param height Image height in pixels.
 * @param sourcePixels An array of RGBA values.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_generateBitmap(
  LPDWORD bitmapResult,
  const DWORD width,
  const DWORD height,
  const LPDWORD sourcePixels);

/**
 * @brief Generates `eTexture` game-object from a source.
 *
 * Remotely allocates memory and constructs `eTexture` game-object
 * from given parameters (bitmap object link).
 * The new object will have its reference counter set to 1.
 * Afterwards the game hangs in an infinite loop until awaken.
 * @param[out] textureResult Where the new `eTexture`
 * game-object's base address will be stored.
 * @param bitmapObject Base address of some `eBitmap` object.
 * Should point to a valid game-object (non-zero value).
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_generateTexture(
  LPDWORD textureResult,
  const DWORD bitmapObject);

/**
 * @brief Generates `eMaterial` game-object from a source.
 *
 * Remotely allocates memory and constructs `eMaterial` game-object
 * from given parameters (material properties and texture object link).
 * The new object will have its reference counter set to 1.
 * Afterwards the game hangs in an infinite loop until awaken.
 * @param[out] materialResult Where the new `eBitmap`
 * game-object's base address will be stored.
 * @param materialFlags Material flags (blending mode, etc.).
 * @param materialCollision Collision type (solid, slippery, lethal, etc.).
 * @param materialSound Sound type (grass, wood, stone, metal, sand).
 * @param materialAlphaRef Alpha reference value (default: `0.5f`).
 * @param textureObject Base address of some `eTexture` object.
 * Should point to a valid game-object (non-zero value).
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_generateMaterial(
  LPDWORD materialResult,
  WORD materialFlags,
  DWORD materialCollision,
  WORD materialSound,
  FLOAT materialAlphaRef,
  const DWORD textureObject);

/**
 * @brief Imitates default constructor of `eNode`-derived game-object.
 *
 * This function runs the algorithm similar to a default constructor
 * (for `eNode`-derived game-objects) found in actual game.
 * The new object will have its reference counter set to 1.
 * @param nodeBase Base address of an existing `eNode` game-object.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_constructNode(const DWORD nodeBase);

/**
 * @brief Imitates default constructor of `eGeometry`-derived game-object.
 *
 * This function runs the algorithm similar to a default constructor
 * (for `eGeometry`-derived game-objects) found in actual game.
 * @param geometryBase Base address of an existing `eGeometry` game-object.
 * @param materialObject Base address of `eMaterial` game-object.
 * Can be set to 0.
 * @param minPoint Reference to a valid `ePoint3` struct (min coord).
 * @param maxPoint Reference to a valid `ePoint3` struct (max coord).
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_constructGeometry(
  const DWORD geometryBase,
  const DWORD materialObject,
  const ePoint3 * minPoint,
  const ePoint3 * maxPoint);

/**
 * @brief Generates `eGeoArray` game-objects that hold a simple cube data.
 *
 * Remotely allocates memory and constructs `eGeoArray` game-objects
 * The new objects will have its reference counters set to 1.
 * Afterwards the game hangs in an infinite loop until awaken.
 * @param[out] stripsPortionsResult Where the new `eGeoArray<ushort>`
 * game-object's base address will be stored.
 * @param[out] indicesArrayResult Where the new `eGeoArray<ushort>`
 * game-object's base address will be stored.
 * @param[out] texCoordsArrayResult Where the new `eGeoArray<ePoint2>`
 * game-object's base address will be stored.
 * @param[out] colorsArrayResult Where the new `eGeoArray<ePoint4>`
 * game-object's base address will be stored.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_generateCubeGeoarrays(
  LPDWORD stripsPortionsResult,
  LPDWORD indicesArrayResult,
  LPDWORD texCoordsArrayResult,
  LPDWORD colorsArrayResult);

/**
 * @brief Generates `eTriMesh` game-object that holds a simple cube data.
 *
 * Remotely allocates memory and constructs `eTriMesh` game-object
 * The new object will have its reference counter set to 1.
 * Afterwards the game hangs in an infinite loop until awaken.
 * @param[out] triMeshResult Where the new `eTriMesh`
 * game-object's base address will be stored.
 * @param materialObject Base address of `eMaterial` game-object.
 * Can be set to 0.
 * @param minPoint Reference to a valid `ePoint3` struct (min coord).
 * @param maxPoint Reference to a valid `ePoint3` struct (max coord).
 * @param stripsPortionsObject Base address of an existing
 * `eGeoArray<ushort>` game-object.
 * @param indicesArrayObject Base address of an existing
 * `eGeoArray<ushort>` game-object.
 * @param texCoordsArrayObject Base address of an existing
 * `eGeoArray<ePoint2>` game-object.
 * @param colorsArrayObject Base address of an existing
 * `eGeoArray<ePoint4>` game-object.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_generateCubeTrimesh(
  LPDWORD triMeshResult,
  const DWORD materialObject,
  const ePoint3 * minPoint,
  const ePoint3 * maxPoint,
  const DWORD stripsPortionsObject,
  const DWORD indicesArrayObject,
  const DWORD texCoordsArrayObject,
  const DWORD colorsArrayObject);

/**
 * @brief Resets vertex colors of some `eTriMesh` game-object.
 *
 * Checks if `eGeoSet` game-object, referenced in the selected `eTriMesh`
 * game-object, contains a reference to a valid `eGeoArray<ePoint4>`
 * game-object which represents colors of each vertex. Then each color
 * struct is replaced with `{1.0f, 1.0f, 1.0f, 1.0f}`.
 * @param triMeshBase Base address of an existing `eTriMesh` game-object.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_resetTrimeshVertexColors(const DWORD triMeshBase);

/**
 * @brief Assigns a compatible material to exiting `eTriMesh` game-object.
 *
 * Compatible `eMaterial` object matches the collision and sound settings
 * of original `eMaterial`, but will be paired with a new `eTexture`.
 * If no material with specific settings exists, it will be generated
 * and pushed into a special array.
 * @param triMeshBase Base address of an existing `eTriMesh` game-object.
 * @param materialStorage Generic array containing `MaterialIdentifierEx`
 * structures.
 * @param textureObject Base address of an existing `eTexture` game-object.
 * @return TRUE on success, FALSE on I/O error.
 */
BOOL KaoObjects_assignCompatibleMaterial(
  const DWORD triMeshBase,
  AnyArray * materialStorage,
  const DWORD textureObject);


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO_WINAPI_TOOLBASE_KAO_OBJECTS */


/**************************************************************/
