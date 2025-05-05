#include "data_diversity_snippet.h"

#include "reliability_mechanisms/single_design/error_detection/data_diversity.h"

// Пример вычисляемой функции
void compute_sum(void* result, const void* input) {
    const uint8_t* data = (const uint8_t*)input;
    uint32_t* sum = (uint32_t*)result;
    *sum = 0;
    for (uint32_t i = 0; i < 5; ++i) {
        *sum += data[i];
    }
}

// Функция сравнения результатов
bool compare_uint32(const void* a, const void* b) {
    return (*(const uint32_t*)a) == (*(const uint32_t*)b);
}

void example_data_diversity_snippet(void) {
    uint8_t input1[5] = { 1, 2, 3, 4, 5 };
    uint8_t input2[5] = { 5, 4, 3, 2, 1 }; // Те же числа в другом порядке
    uint8_t input3[5] = { 3, 1, 2, 5, 4 }; // Перемешано

    const void* inputs[] = { input1, input2, input3 };

    uint32_t result1, result2, result3;
    void* results[] = { &result1, &result2, &result3 };

    bool ok = data_diversity_execute(
        compute_sum,
        compare_uint32,
        inputs,
        3,
        results,
        VOTING_STRICT_COMPARE,
        __func__
    );

    if (!ok) {
    } else {
    }
}

void data_diversity_majority_snippet(void) {
    uint8_t input1[5] = { 1, 2, 3, 4, 5 };
    uint8_t input2[5] = { 5, 4, 3, 2, 1 }; // Переставлено
    uint8_t input3[5] = { 1, 2, 3, 4, 5 }; // Оригинал

    const void* inputs[] = { input1, input2, input3 };

    uint32_t result1, result2, result3;
    void* results[] = { &result1, &result2, &result3 };

    bool ok = data_diversity_execute(
        compute_sum,
        compare_uint32,
        inputs,
        3,
        results,
        VOTING_MAJORITY,
        __func__
    );

    if (!ok) {
        printf("Data diversity: majority mismatch detected!\n");
    } else {
        printf("Data diversity: majority vote OK. Sum = %u\n", result1);
    }
}