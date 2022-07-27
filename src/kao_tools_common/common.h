/**************************************************************/
/* "kao_tools_common/common.h"                                */
/**************************************************************/

#ifndef H_KAO_TOOLS_COMMON
#define H_KAO_TOOLS_COMMON

#include <stdint.h>

/**
 * @defgroup ToolBase_Common ToolBase :: Common
 * @{
 */


/**************************************************************/
/* Static assertions                                          */
/**************************************************************/

/**
 * @def STATIC_ASSERT_GLUE(a, b)
 * Macro for building unique identifier for a Static Assertion.
 * @def STATIC_ASSERT_VERIFY(_expr, _line)
 * Preprocessor generates a special `typedef`, that is only valid when
 * the `_expr` is TRUE. Otherwise this will produce a compilation error.
 * @def STATIC_ASSERT(_expr)
 * Generates a static (compile-time) assertion from given boolean expression.
 */

#define STATIC_ASSERT_GLUE(a, b) \
  a##__##b

#define STATIC_ASSERT_VERIFY(_expr, _line) \
  typedef char STATIC_ASSERT_GLUE(compiler_verify, _line) \
  [(_expr) ? (+1) : (-1)];

#define STATIC_ASSERT(_expr) \
  STATIC_ASSERT_VERIFY(_expr, __LINE__)


/**************************************************************/
/* Helper definitions                                         */
/**************************************************************/

/**
 * @def FALSE
 * Standard macro for a `false` (zero) boolean value.
 * @def TRUE
 * Standard macro for a `true` (non-zero) boolean value.
 */

#if !defined(_WINDOWS_)
  #define FALSE 0
  #define TRUE  1
  typedef int      BOOL;
  typedef uint8_t  BYTE;
  typedef BYTE     BOOLEAN;
  typedef uint16_t WORD;
  typedef uint32_t DWORD;
#endif

STATIC_ASSERT(1 == sizeof(BYTE))
STATIC_ASSERT(1 == sizeof(BOOLEAN))
STATIC_ASSERT(2 == sizeof(WORD))
STATIC_ASSERT(4 == sizeof(DWORD))


/**************************************************************/
/* Elephant Engine :: Type definitions                        */
/**************************************************************/

/**
 * @brief eQuat type.
 */
typedef struct e_quat_t eQuat;

/**
 * @brief Quaternion structure.
 */
struct e_quat_t
{
  float x;  /**< x component (imaginary) */
  float y;  /**< y component (imaginary) */
  float z;  /**< z component (imaginary) */
  float w;  /**< w component (real) */
};

/* eQuat size should be exactly 16 bytes */
STATIC_ASSERT((4 * 4) == sizeof(eQuat))

/**
 * @brief ePoint2 type.
 */
typedef struct e_point2_t ePoint2;

/**
 * @brief ePoint2 structure.
 */
struct e_point2_t
{
  float x;  /**< x-coordinate */
  float y;  /**< y-coordinate */
};

/* ePoint2 size should be exactly 8 bytes */
STATIC_ASSERT((2 * 4) == sizeof(ePoint2))

/**
 * @brief ePoint3 type.
 */
typedef struct e_point3_t ePoint3;

/**
 * @brief ePoint3 structure.
 */
struct e_point3_t
{
  float x;  /**< x-coordinate */
  float y;  /**< y-coordinate */
  float z;  /**< z-coordinate */
};

/* ePoint3 size should be exactly 12 bytes */
STATIC_ASSERT((3 * 4) == sizeof(ePoint3))

/**
 * @brief ePoint4 type.
 */
typedef struct e_point4_t ePoint4;

/**
 * @brief ePoint4 structure.
 */
struct e_point4_t
{
  float x;  /**< x-coordinate */
  float y;  /**< y-coordinate */
  float z;  /**< z-coordinate */
  float w;  /**< w-coordinate */
};

/* ePoint4 size should be exactly 16 bytes */
STATIC_ASSERT((4 * 4) == sizeof(ePoint4))

/**
 * @brief eSRP (Scale Rotation Position) type.
 */
typedef struct e_srp_t eSRP;

/**
 * @brief eSRP (Scale Rotation Position) structure.
 */
struct e_srp_t
{
  eQuat   rot;  /**< Rotation */
  ePoint3 pos;  /**< Position */
  float   scl;  /**< Scale */
};

/* eSRP size should be exactly 32 bytes */
STATIC_ASSERT((4 * (4 + 3 + 1)) == sizeof(eSRP))


/**************************************************************/
/* Helper macros                                              */
/**************************************************************/

/**
 * @def EQ(__a, __b)
 * Tests if two terms (variables, constants) are equal.
 * @def NE(__a, __b)
 * Tests if two terms (variables, constants) are not equal.
 * @def IS_NULL(__a)
 * Tests if term (variable) `__a` is set to an invalid (NULL) pointer.
 * @def NOT_NULL(__a)
 * Tests if term (variable) `__a` is different from an invalid (NULL) pointer.
 * @def IS_ZERO(__a)
 * Tests if term (variable) `__a` is equal to ZERO.
 * @def NOT_ZERO(__a)
 * Tests if term (variable) `__a` is different from ZERO.
 * @def IS_FALSE(__a)
 * Tests if term (boolean variable) `__a` is equal to FALSE.
 * @def NOT_FALSE(__a)
 * Tests if term (boolean variable) `__a` is different from FALSE.
 * @def IS_TRUE(__a)
 * Tests if term (boolean variable) `__a` is equal to TRUE.
 * @def NOT_TRUE(__a)
 * Tests if term (boolean variable) `__a` is different from TRUE.
 * @def FAIL_WHEN(__cond)
 * If given condition `__cond` evaluates to TRUE, then skip to `leave` label
 * (local label defined in any caller function).
 * @def TRY_BOOL(__a)
 * Evaluates term (return value from a function call, expression) `__a`.
 * If it is TRUE, jump to the local `leave` label.
 */

#define EQ(__a, __b)  ((__a) == (__b))
#define NE(__a, __b)  ((__a) != (__b))

#define   IS_NULL(__a)  EQ((void *)0, __a)
#define  NOT_NULL(__a)  NE((void *)0, __a)
#define   IS_ZERO(__a)  EQ(0,         __a)
#define  NOT_ZERO(__a)  NE(0,         __a)
#define  IS_FALSE(__a)  EQ(FALSE,     __a)
#define NOT_FALSE(__a)  NE(FALSE,     __a)
#define   IS_TRUE(__a)  EQ(TRUE,      __a)
#define  NOT_TRUE(__a)  NE(TRUE,      __a)

#define FAIL_WHEN(__cond)  if __cond { goto leave; }

#define TRY_BOOL(__a)  FAIL_WHEN(IS_FALSE(__a))


/**************************************************************/

/**
 * @}
 */

#endif  /* H_KAO_TOOLS_COMMON */


/**************************************************************/
