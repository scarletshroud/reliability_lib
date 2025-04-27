#ifndef TEST_MODEL_H
#define TEST_MODEL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t num_errors;    // Количество обнаруженных ошибок
    float *intervals;       // Массив интервалов между ошибками
    float a;                // Параметр a модели
    float b;                // Параметр b модели
    float reliability;      // Текущая оценка надежности
} test_model_t;

// Инициализация модели тестов
void test_model_init(test_model_t *model, uint32_t num_errors);

// Обновление интервалов между ошибками
void test_model_update_intervals(test_model_t *model, float *new_intervals);

// Оценка параметров модели (a и b)
void test_model_estimate_parameters(test_model_t *model);

// Расчет текущей надежности
void test_model_calculate_reliability(test_model_t *model);

#ifdef __cplusplus
}
#endif

#endif // TEST_MODEL_H