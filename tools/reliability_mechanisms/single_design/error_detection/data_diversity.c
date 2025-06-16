#include "data_diversity.h"

#include <stdlib.h>

#include "tools/tools_config.h"

#ifdef ERROR_MONITOR_ENABLE
#include "error_monitor/error_monitor.h"
#endif

uint8_t data_diversity_execute(
    void (*target_fn)(void* result, const void* input),
    data_diversity_compare_fn compare,
    const void** input_variants,
    uint32_t input_count,
    void** result_buffers,
    voting_mode_t voting,
    const char* context
) {
    if (!target_fn || !compare || !input_variants || !result_buffers || input_count < 2) {
        return 0;
    }

    // Выполнить функцию для всех вариантов входных данных
    for (uint32_t i = 0; i < input_count; ++i) {
        target_fn(result_buffers[i], input_variants[i]);
    }

    if (voting == VOTING_STRICT_COMPARE) {
        for (uint32_t i = 1; i < input_count; ++i) {
            if (!compare(result_buffers[0], result_buffers[i])) {

#ifdef ERROR_MONITOR_ENABLE
                error_monitor_save_event(__FILE__, context, "Data diversity strict mismatch", __LINE__, ERROR_LEVEL_ERROR);
#endif

                return 0;
            }
        }
    } else if (voting == VOTING_MAJORITY) {
        uint32_t votes = 1; // результат #0
        for (uint32_t i = 1; i < input_count; ++i) {
            if (compare(result_buffers[0], result_buffers[i])) {
                votes++;
            }
        }
        if (votes <= (input_count / 2)) {
#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, context, "Data diversity majority mismatch", __LINE__, ERROR_LEVEL_ERROR);
#endif
            return 0;
        }
    }

    return 1;
}

// Простейшее перемешивание массива (Fisher-Yates shuffle)
void data_diversity_shuffle(uint8_t* data, uint32_t len) {
    if (!data || len == 0) return;
    for (uint32_t i = len - 1; i > 0; --i) {
        uint32_t j = rand() % (i + 1);
        uint8_t tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;
    }
}

// Добавление псевдошума (прибавить или убавить случайные значения)
void data_diversity_add_noise(uint8_t* data, uint32_t len, uint8_t noise_level) {
    if (!data || len == 0) return;
    for (uint32_t i = 0; i < len; ++i) {
        int8_t noise = (rand() % (2 * noise_level + 1)) - noise_level;
        int16_t new_val = (int16_t)data[i] + noise;
        if (new_val < 0) new_val = 0;
        if (new_val > 255) new_val = 255;
        data[i] = (uint8_t)new_val;
    }
}
