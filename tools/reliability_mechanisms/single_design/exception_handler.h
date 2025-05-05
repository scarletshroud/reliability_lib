#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EXCEPTION_RAISE(ex_type) exception_raise(ex_type, __FILE__, __func__, __LINE__)

    typedef enum {
        EXCEPTION_NONE = 0,
        EXCEPTION_ASSERTION_FAILED,
        EXCEPTION_DIVISION_BY_ZERO,
        EXCEPTION_DEADLINE_EXCEEDED,
        EXCEPTION_MEMORY_CORRUPTION,
        EXCEPTION_UNKNOWN_ERROR
    } ExceptionType;

    typedef void (*exception_handler_fn)(ExceptionType ex_type, const char* file, const char* func, uint32_t line);

    /// Инициализация модуля исключений
    void exception_handler_init(exception_handler_fn handler);

    /// Генерация исключения
    void exception_raise(ExceptionType ex_type, const char* file, const char* func, uint32_t line);

#ifdef __cplusplus
}
#endif

#endif // EXCEPTION_HANDLER_H
