#ifndef DATA_PROPERTY_CHECKER_H
#define DATA_PROPERTY_CHECKER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef bool (*property_check_fn_t)(const void* structure);

    typedef struct {
        const char* property_name;          // Название проверяемого свойства
        property_check_fn_t check_function; // Указатель на функцию проверки
    } property_checker_t;

    /// Проверка одного свойства структуры
    bool data_property_check_single(const property_checker_t* checker, const void* structure);

    /// Проверка всех свойств структуры
    bool data_property_check_all(const property_checker_t* checkers, uint32_t count, const void* structure);

#ifdef __cplusplus
}
#endif

#endif // DATA_PROPERTY_CHECKER_H
