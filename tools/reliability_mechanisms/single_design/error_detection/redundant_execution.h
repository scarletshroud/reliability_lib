#ifndef REDUNDANT_EXECUTION_H
#define REDUNDANT_EXECUTION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* Указатель на функцию валидации */
    typedef uint8_t (*redundant_compare_fn)(const void* result1, const void* result2);

    /**
     * \brief           Функция дублирования вычисления функции без параметров
     * \param[in]       target_fn: Указатель на функцию для дублирования
     * \param[in]       compare: Указатель на компаратор
     * \param[in]       result_buffer1: Указатель на буфер для хранения первого результата
     * \param[in]       result_buffer2: Указатель на буфер для хранения второго результата
     * \param[in]       context: Дополнительная информация о контексте
     * \return          Результат проверки: 0 - ошибка, 1 - успешно
    */
    uint8_t redundant_execute_noargs(
        void (*target_fn)(void* result),
        redundant_compare_fn compare,
        void* result_buffer1,
        void* result_buffer2,
        const char* context
    );
    
    /**
     * \brief           Функция дублирования вычисления функции с аргументом
     * \param[in]       target_fn: Указатель на функцию для дублирования
     * \param[in]       compare: Указатель на компаратор
     * \param[in]       input_arg: Указатель на входные параметры
     * \param[in]       result_buffer1: Указатель на буфер для хранения первого результата
     * \param[in]       result_buffer2: Указатель на буфер для хранения второго результата
     * \param[in]       context: Дополнительная информация о контексте
     * \return          Результат проверки: 0 - ошибка, 1 - успешно
    */
    uint8_t redundant_execute_with_args(
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
