#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EXCEPTION_RAISE(ex_type) exception_raise(ex_type, __FILE__, __func__, __LINE__)

    typedef enum {  
        EXCEPTION_NONE = 0,
        EXCEPTION_ASSERTION_FAILED,     /* Срабатывание Assertion */
        EXCEPTION_DIVISION_BY_ZERO,     /* Деление на ноль */
        EXCEPTION_DEADLINE_EXCEEDED,    /* Нарушение времени исполнения */
        EXCEPTION_MEMORY_CORRUPTION,    /* Нарушение памяти */
        EXCEPTION_UNKNOWN_ERROR         /* Неизвестный тип ошибки */
    } ExceptionType;

    /* Указатель на обработчик исключения */
    typedef void (*exception_handler_fn)(ExceptionType ex_type, const char* file, const char* func, uint32_t line);

    /**
     * @brief           Функция инициализации модуля исключений
     * @param[in]       handler: Обработчик исключения
    */
    void exception_handler_init(exception_handler_fn handler);

    /**
     * @brief           Функция генерации исключения
     * @param[in]       ex_type: Тип исключения
     * @param[in]       file: Текущий файл исходного кода
     * @param[in]       func: Текущая функция
     * @param[in]       line: Текущая линия исходного кода
    */
    void exception_raise(ExceptionType ex_type, const char* file, const char* func, uint32_t line);

#ifdef __cplusplus
}
#endif

#endif // EXCEPTION_HANDLER_H
