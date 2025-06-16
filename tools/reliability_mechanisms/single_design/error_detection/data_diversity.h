#ifndef DATA_DIVERSITY_H
#define DATA_DIVERSITY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Указатель на функцию компаратор двух результатов работы целевой функции */
typedef uint8_t (*data_diversity_compare_fn)(const void* result1, const void* result2);

typedef enum {
    VOTING_STRICT_COMPARE,  /* Все результаты должны быть одинаковыми */
    VOTING_MAJORITY         /* Большинство результатов должно совпадать */
} voting_mode_t;

/// 
/**
 * @brief           Функция для выполнения функции с различными входными данными и сравнения их результов
 * @param[in]       target_fn: Указатель на целевую функцию
 * @param[in]       compare: Функция-компаратор
 * @param[in]       input_variants: Различные наборы входных данных
 * @param[in]       input_count: Количество входов
 * @param[in]       result_buffers: Выходные данные
 * @param[in]       voting: Режим голосования
 * @param[in]       context: Контекст выполнения
 * @return          Результат проверки: 0 - ошибка, 1 - успешно
*/
uint8_t data_diversity_execute(
    void (*target_fn)(void* result, const void* input),
    data_diversity_compare_fn compare,
    const void** input_variants,
    uint32_t input_count,
    void** result_buffers,
    voting_mode_t voting,
    const char* context
);

/* Алгоритмы создания вариаций данных */
void data_diversity_shuffle(uint8_t* data, uint32_t len);
void data_diversity_add_noise(uint8_t* data, uint32_t len, uint8_t noise_level);

#ifdef __cplusplus
}
#endif

#endif // DATA_DIVERSITY_H
