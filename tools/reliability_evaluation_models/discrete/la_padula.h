#ifndef RELIABILITY_LAPADULA_H
#define RELIABILITY_LAPADULA_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LAPADULA_MAX_STEPS 128

typedef struct {
    uint32_t S[LAPADULA_MAX_STEPS]; // Количество тестов на этапе
    uint32_t m[LAPADULA_MAX_STEPS]; // Количество ошибок на этапе
    size_t step_count;              // Количество этапов
} LapadulaModel;

typedef struct {
    double R_inf;
    double A;
    double R_inf_std_err; // стандартная ошибка оценки R(∞)
    double A_std_err;     // стандартная ошибка оценки A
} LapadulaResult;

/// Инициализация модели
void lapadula_init(LapadulaModel* model);

/// Добавить данные этапа
void lapadula_add_step(LapadulaModel* model, uint32_t total_tests, uint32_t errors);

/// Решение системы для нахождения R(∞) и A, а также расчёт стандартных ошибок
void lapadula_solve(const LapadulaModel* model, LapadulaResult* result);

/// Оценка надёжности на этапе i
double lapadula_reliability(double R_inf, double A, uint32_t i);

#ifdef __cplusplus
}
#endif

#endif // RELIABILITY_LAPADULA_H