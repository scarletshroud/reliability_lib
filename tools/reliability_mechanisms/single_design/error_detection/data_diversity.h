#ifndef DATA_DIVERSITY_H
#define DATA_DIVERSITY_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*data_diversity_compare_fn)(const void* result1, const void* result2);

typedef enum {
    VOTING_STRICT_COMPARE,  // Все результаты должны быть одинаковыми
    VOTING_MAJORITY         // Большинство результатов должно совпадать
} voting_mode_t;

/// Выполнить функцию с различными входными данными и сравнить результаты
bool data_diversity_execute(
    void (*target_fn)(void* result, const void* input),
    data_diversity_compare_fn compare,
    const void** input_variants,
    uint32_t input_count,
    void** result_buffers,
    voting_mode_t voting,
    const char* context
);

/// Алгоритмы создания вариаций данных
void data_diversity_shuffle(uint8_t* data, uint32_t len);
void data_diversity_add_noise(uint8_t* data, uint32_t len, uint8_t noise_level);

#ifdef __cplusplus
}
#endif

#endif // DATA_DIVERSITY_H
