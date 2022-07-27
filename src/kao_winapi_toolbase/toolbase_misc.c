/**************************************************************/
/* "kao_winapi_toolbase/toolbase_misc.c"                      */
/**************************************************************/

#include "toolbase_misc.h"

/**
 * @addtogroup ToolBase_Misc
 * @{
 */


/**************************************************************/
/* Misc :: Public functions                               */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
float Misc_clampf(const float x, const float a, const float b)
{
  return (x < a) ? a : ((x > b) ? b : x);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
float Misc_linearMapping(
  const float x,
  const float a, const float b,
  const float c, const float d)
{
  const float y = (x - a) / (b - a) * (d - c) + c;
  return (c <= d) ? Misc_clampf(y, c, d) : Misc_clampf(y, d, c);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
LPDWORD
Misc_generateCheckerImage(
  const COLORREF color,
  const size_t width, const size_t height,
  const float alphaOut, const float alphaIn)
{
  size_t x, y;
  DWORD tmpColor;

  const size_t half_width = width / 2;
  const size_t half_height = height / 2;
  const size_t image_size = width * height;

  const float mapB = ((float) (half_width + half_height)) / 2.0f;
  float mapX;

  const float midRed   = GetRValue(color) / 255.0f;
  const float midGreen = GetGValue(color) / 255.0f;
  const float midBlue  = GetBValue(color) / 255.0f;
  const float midAlpha = alphaIn * 255.0f;

  const float edgeRed   = midRed   * 0.25f;
  const float edgeGreen = midGreen * 0.25f;
  const float edgeBlue  = midBlue  * 0.25f;
  const float edgeAlpha = alphaOut * 255.0f;

  ePoint4 dummyColor;
  ePoint4 *colorPointer;
  ePoint4 *point4Image;
  LPDWORD dwordImage;

  point4Image = (ePoint4 *) malloc(sizeof(ePoint4) * image_size);

  if IS_NULL(point4Image)
  {
    return NULL;
  }

  dwordImage = (LPDWORD) malloc(sizeof(DWORD) * image_size);

  if IS_NULL(dwordImage)
  {
    free(point4Image);
    return NULL;
  }

  for (y = 0; y < half_height; y++)
  {
    for (x = 0; x < half_width; x++)
    {
      mapX = ((float) x) + ((float) y) / 2;

      dummyColor.x = Misc_linearMapping(mapX, 0, mapB, edgeRed,   midRed);
      dummyColor.y = Misc_linearMapping(mapX, 0, mapB, edgeGreen, midGreen);
      dummyColor.z = Misc_linearMapping(mapX, 0, mapB, edgeBlue,  midBlue);
      dummyColor.w = Misc_linearMapping(mapX, 0, mapB, edgeAlpha, midAlpha);

      point4Image[                  y  * width     + x] = dummyColor;
      point4Image[             (y + 1) * width - 1 - x] = dummyColor;
      point4Image[image_size - (y + 1) * width     + x] = dummyColor;
      point4Image[image_size      - y  * width - 1 - x] = dummyColor;
    }
  }

  for (y = 0; y < width; y++)
  {
    for (x = 0; x < height; x++)
    {
      mapX = (((x * 8 / width) + (y * 8 / height)) % 2) ? 0.125f : 0;
      colorPointer = &(point4Image[y * width + x]);

      tmpColor = MISC_RGBA_DWORD(
        (BYTE) (Misc_clampf(colorPointer->x + mapX, 0, 1.f) * 255.f),
        (BYTE) (Misc_clampf(colorPointer->y + mapX, 0, 1.f) * 255.f),
        (BYTE) (Misc_clampf(colorPointer->z + mapX, 0, 1.f) * 255.f),
        (BYTE) (colorPointer->w));

      dwordImage[y * width + x] = tmpColor;
    }
  }

  free(point4Image);
  return dwordImage;
}


/**************************************************************/

/**
 * @}
 */


/**************************************************************/
