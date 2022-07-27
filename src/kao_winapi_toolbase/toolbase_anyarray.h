/**************************************************************/
/* "kao_winapi_toolbase/toolbase_anyarray.h"                  */
/**************************************************************/

#ifndef H_KAO_WINAPI_TOOLBASE_ANYARRAY
#define H_KAO_WINAPI_TOOLBASE_ANYARRAY

#include "toolbase_main.h"

/**
 * @defgroup ToolBase_AnyArray ToolBase :: AnyArray
 * @{
 */


/**************************************************************/
/* Any Array :: Type definitions                              */
/**************************************************************/

/**
 * @brief AnyArray type.
 */
typedef struct any_array_t AnyArray;

/**
 * @brief AnyArray structure.
 * Array of elements of unspecified type (only specified element size).
 */
struct any_array_t
{
  size_t elementSize;    /**< Size of one element, in bytes. */
  size_t currentSize;    /**< Number of valid elements. */
  size_t allocatedSize;  /**< Number of elements fitting into the array. */
  LPVOID elements;       /**< Dynamically allocated array of generic things. */
};


/**************************************************************/
/* Any Array :: Public functions                              */
/**************************************************************/

/**
 * @brief AnyArray constructor.
 *
 * @param me AnyArray object reference (uninitialized).
 * @param elementSize Size of a single element (in bytes).
 */
VOID AnyArray_init(AnyArray * me, const size_t elementSize);

/**
 * @brief AnyArray destructor.
 *
 * @param me AnyArray object reference.
 */
VOID AnyArray_destroy(AnyArray * me);

/**
 * @brief Append a generic element to the AnyArray.
 *
 * @param me AnyArray object reference.
 * @param element Element to be copied and placed at the end of the list.
 * @return TRUE on success, FALSE on memory allocation error.
 */
BOOL AnyArray_append(AnyArray * me, LPCVOID element);

/**
 * @brief Find first matching element.
 *
 * @param me AnyArray object reference.
 * @param comparedElement Element sample, from which some subpart is compared.
 * @param startOffset Starting offset for element comparison.
 * [0 >= startOffset < elementSize]
 * @param numValidBytes Number of bytes to bo compared betwen elements.
 * [1 >= numValidBytes <= (elementSize - startOffset)]
 * @return Index of the first element that matches a subset of bytes
 * from `comparedElement`, or (-1) if no match was found.
 */
INT AnyArray_partialMatch(
  const AnyArray * me,
  LPCVOID comparedElement,
  size_t startOffset,
  size_t numValidBytes);

/**
 * @brief Get i-th element from the array.
 *
 * @param me AnyArray object reference.
 * @param[out] output Byte array to write the results to.
 * @param idx element index (zero-based).
 */
VOID AnyArray_getIthElement(
  const AnyArray * me,
  LPVOID output,
  size_t idx);

/**
 * @brief Get i-th element from the array.
 *
 * @param me AnyArray object reference.
 * @param input Byte array to read data from.
 * @param idx element index (zero-based).
 */
VOID AnyArray_setIthElement(
  AnyArray * me,
  LPCVOID input,
  size_t idx);


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO_WINAPI_TOOLBASE_ANYARRAY */


/**************************************************************/
