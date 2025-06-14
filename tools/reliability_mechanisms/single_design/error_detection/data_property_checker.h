#ifndef DATA_PROPERTY_CHECKER_H
#define DATA_PROPERTY_CHECKER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* Указатель на функцию проверки */
    typedef uint8_t (*property_check_fn_t)(const void* structure);

    typedef struct {
        const char* property_name;          /* Название проверяемого свойства */
        property_check_fn_t check_function; /* Указатель на функцию проверки */
    } property_checker_t;

     /**
     * \brief           Функция проверки одного свойства структуры
     * \param[in]       checker: Указатель на функцию проверкий
     * \param[in]       structure: Указатель на проверяемую структуру
     * \return          Результат проверки: 0 - ошибка, 1 - успешно
    */
    uint8_t data_property_check_single(const property_checker_t* checker, const void* structure);

    /**
     * \brief           Функция проверки всех свойств структуры
     * \param[in]       checkers: Указатель на массив функций-проверок
     * \param[in]       count: Количество полей
     * \param[in]       structure: Указатель на проверяемую структуру
     * \return          Результат проверки: 0 - ошибка, 1 - успешно
    */
    uint8_t data_property_check_all(const property_checker_t* checkers, uint32_t count, const void* structure);

#ifdef __cplusplus
}
#endif

#endif // DATA_PROPERTY_CHECKER_H
