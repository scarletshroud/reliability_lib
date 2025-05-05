#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef bool (*validation_fn_t)(const void* value, const void* context);

    typedef struct {
        const char* field_name;          // Название проверяемого поля
        validation_fn_t validate;        // Функция валидации
        const void* context;             // Дополнительный контекст для проверки (например, диапазон)
    } validator_t;

    /// Проверка одного поля
    bool input_validate_field(const validator_t* validator, const void* value);

    /// Множественная проверка набора полей
    bool input_validate_all(const validator_t* validators, uint32_t count, const void** values);

#ifdef __cplusplus
}
#endif

#endif // INPUT_VALIDATION_H
