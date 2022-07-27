/**************************************************************/
/* "kao_winapi_toolbase/toolbase_misc.h"                      */
/**************************************************************/

#ifndef H_KAO_WINAPI_TOOLBASE_MISC
#define H_KAO_WINAPI_TOOLBASE_MISC

#include "toolbase_main.h"

/**
 * @defgroup ToolBase_Misc ToolBase :: Misc
 * @{
 */


/**************************************************************/
/* Misc :: Constants                                          */
/**************************************************************/

/**
 * @def MISC_DEFAULT_IMAGE_WIDTH
 * Default width for images generated with ToolBase Misc functions.
 */

#define MISC_DEFAULT_IMAGE_WIDTH  64


/**************************************************************/
/* Misc :: Helper macros                                      */
/**************************************************************/

/**
 * @def MISC_RGBA_DWORD(__r, __g, __b, __a)
 * Constructs 32-bit Little-Endian integer from four
 * color components (red, green, blue, alpha).
 */

#define MISC_RGBA_DWORD(__r, __g, __b, __a) \
  (((__a) << 24) | ((__b) << 16) | ((__g) << 8) | (__r))


/**************************************************************/
/* Misc :: Public functions                                   */
/**************************************************************/

/**
 * @brief Clamp floating-point value to some range.
 *
 * @param x The floating-point value to be tested.
 * @param a Minimum value (range start, inclusive).
 * @param b Maximum value (range end, inclusive).
 * @return Clamped floating-point value.
 */
float Misc_clampf(const float x, const float a, const float b);

/**
 * @brief Mapping floating-point value X from range [A,B] to range [C,D].
 *
 * @param x The floating-point value to be tested (falls in range [A,B]).
 * @param a Source range start.
 * @param b Source range end.
 * @param c Destination range start.
 * @param d Destination range end.
 * @return Mapped floating-point value.
 */
float Misc_linearMapping(
  const float x,
  const float a, const float b,
  const float c, const float d);

/**
 * @brief Generates dynamically allocated RGBA image with specific tint.
 *
 * @param color Base color (middle of the image).
 * @param width Image width in pixels.
 * @param height Image height in pixels.
 * @param alphaOut Alpha value on edges [0.0 to 1.0].
 * @param alphaIn Alpha value in the middle [0.0 to 1.0].
 * @return Array of RGBA values (NULL on memory allocation error).
 */
LPDWORD Misc_generateCheckerImage(
  const COLORREF color,
  const size_t width, const size_t height,
  const float alphaOut, const float alphaIn);


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO_WINAPI_TOOLBASE_MISC */


/**************************************************************/
