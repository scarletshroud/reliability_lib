#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* Указатель на функцию валидации */
    typedef uint8_t (*validation_fn_t)(const void* value, const void* context);

    typedef struct {
        const char* field_name;          /* Название проверяемого поля */
        validation_fn_t validate;        /* Функция валидации */
        const void* context;             /* Дополнительный контекст для проверки (например, диапазон) */
    } validator_t;

    /**
     * \brief           Функция проверки одного поля
     * \param[in]       validator: Указатель на валидатор
     * \param[in]       value: Указатель на поле
     * \return          Результат проверки: 0 - ошибка, 1 - успешно
    */
    uint8_t input_validate_field(const validator_t* validator, const void* value);

    /**
     * \brief           Функция множественной проверки набора полей
     * \param[in]       validators: Указатель на массив валидаторов
     * \param[in]       value: Указатель на массив полей
     * \return          Результат проверки: 0 - ошибка, 1 - успешно
    */
    uint8_t input_validate_all(const validator_t* validators, uint32_t count, const void** values);

#ifdef __cplusplus
}
#endif

#endif // INPUT_VALIDATION_H
