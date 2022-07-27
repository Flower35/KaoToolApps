/**************************************************************/
/* "kao_winapi_toolbase/toolbase_anyarray.c"                  */
/**************************************************************/

#include "toolbase_anyarray.h"

/**
 * @addtogroup ToolBase_AnyArray
 * @{
 */


/**************************************************************/
/* Any Array :: Public functions                              */
/**************************************************************/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
AnyArray_init(AnyArray * me, const size_t elementSize)
{
  me->elementSize = elementSize;

  me->currentSize   = 0;
  me->allocatedSize = 0;

  me->elements = NULL;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
AnyArray_destroy(AnyArray * me)
{
  if NOT_NULL(me->elements)
  {
    free(me->elements);
    me->elements = NULL;
  }

  me->allocatedSize = 0;
  me->currentSize   = 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
BOOL
AnyArray_append(AnyArray * me, LPCVOID element)
{
  LPVOID new_elements;
  const size_t old_size = me->currentSize;
  const size_t old_block_size = (me->elementSize) * old_size;
  size_t new_size = 1 + old_size;

  if (new_size >= (me->allocatedSize))
  {
    new_size = (me->allocatedSize) ? (2 * (me->allocatedSize)) : 1;
    new_elements = malloc((me->elementSize) * new_size);

    if IS_NULL(new_elements)
    {
      return FALSE;
    }

    if NOT_NULL(me->elements)
    {
      memcpy(new_elements, me->elements, old_block_size);
      free(me->elements);
    }

    me->elements = new_elements;
    me->allocatedSize = new_size;
  }

  memcpy(
    &(((LPBYTE) me->elements)[old_block_size]),
    element,
    me->elementSize);

  me->currentSize += 1;
  return TRUE;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
INT
AnyArray_partialMatch(
  const AnyArray * me,
  LPCVOID comparedElement,
  size_t startOffset,
  size_t numValidBytes)
{
  size_t i;
  LPCVOID nextElement;

  if (
    (startOffset >= (me->elementSize)) ||
    (numValidBytes < 1) ||
    (numValidBytes > (me->elementSize - startOffset)))
  {
    return (-1);
  }

  nextElement = me->elements;

  for (i = 0; i < (me->currentSize); i++)
  {
    if IS_ZERO(
      memcmp(
        &(((const BYTE *) nextElement)[startOffset]),
        &(((const BYTE *) comparedElement)[startOffset]),
        numValidBytes))
    {
      return i;
    }

    nextElement = ((LPBYTE) nextElement) + (me->elementSize);
  }

  return (-1);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
AnyArray_getIthElement(
  const AnyArray * me,
  LPVOID output,
  size_t idx)
{
  // (...) if (idx >= (me->currentSize)) { return FALSE; }

  memcpy(
    &(((LPBYTE) output)[0]),
    &(((const BYTE *) me->elements)[idx * (me->elementSize)]),
    me->elementSize
  );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
VOID
AnyArray_setIthElement(
  AnyArray * me,
  LPCVOID input,
  size_t idx)
{
  // (...) if (idx >= (me->currentSize)) { return FALSE; }

  memcpy(
    &(((LPBYTE) me->elements)[idx * (me->elementSize)]),
    &(((const BYTE *) input)[0]),
    me->elementSize
  );
}

/**************************************************************/

/**
 * @}
 */


/**************************************************************/
