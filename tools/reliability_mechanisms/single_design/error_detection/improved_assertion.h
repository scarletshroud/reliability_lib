#ifndef IMPROVED_ASSERTION_H_
#define IMPROVED_ASSERTION_H_

#include "tools_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ASSERT_LEVEL_DEBUG, // (it's the default, the usual assert)
    ASSERT_LEVEL_WARNING, // (it's just a warning, the program can continue without user intervention)
    ASSERT_LEVEL_ERROR, // an error
    ASSERT_LEVEL_CRITICAL // (it's fatal, it's most likely the program/system got unstable).
} assert_level_t;

typedef enum {
    ASSERT_OK
} assert_op_status_t;

#if ENABLE_ASSERTS
#define ASSERT(expr, level)                                                         \
    do {                                                                            \
        if (!(expr)) {                                                              \
        reliability_assert_handler(#expr, __FILE__, __LINE__, level);               \
        }                                                                           \
    } while (0)                                                                     
#else
#define ASSERT(expr, level) ((void)0)
#endif

assert_op_status_t reliability_assert_handler(const char* expr_str, const char* file, int line, assert_level_t level);

#ifdef __cplusplus
}
#endif

#endif
