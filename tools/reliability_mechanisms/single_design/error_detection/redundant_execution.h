#ifndef REDUNDANT_EXECUTION_H
#define REDUNDANT_EXECUTION_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef bool (*redundant_compare_fn)(const void* result1, const void* result2);

    /// Дублирование вычисления функции без параметров
    bool redundant_execute_noargs(
        void (*target_fn)(void* result),
        redundant_compare_fn compare,
        void* result_buffer1,
        void* result_buffer2,
        const char* context
    );

    /// Дублирование вычисления функции с аргументом
    bool redundant_execute_with_args(
        void (*target_fn)(void* result, const void* arg),
        redundant_compare_fn compare,
        const void* input_arg,
        void* result_buffer1,
        void* result_buffer2,
        const char* context
    );

#ifdef __cplusplus
}
#endif

#endif // REDUNDANT_EXECUTION_H
