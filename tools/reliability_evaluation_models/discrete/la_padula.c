#include "la_padula.h"

#include <string.h>
#include <math.h>

void lapadula_init(LapadulaModel* model) {
    memset(model, 0, sizeof(LapadulaModel));
}

void lapadula_add_step(LapadulaModel* model, uint32_t total_tests, uint32_t errors) {
    if (model->step_count < LAPADULA_MAX_STEPS) {
        model->S[model->step_count] = total_tests;
        model->m[model->step_count] = errors;
        model->step_count++;
    }
}

void lapadula_solve(const LapadulaModel* model, LapadulaResult* result) {
    double sum_x = 0.0;      // i
    double sum_y = 0.0;      // (S_i - m_i)/S_i
    double sum_xx = 0.0;     // i*i
    double sum_xy = 0.0;     // i*(S_i - m_i)/S_i
    double n = (double)model->step_count;

    for (size_t i = 0; i < model->step_count; ++i) {
        double Si = (double)model->S[i];
        double mi = (double)model->m[i];
        double idx = (double)(i + 1); // нумерация от 1

        double y = (Si - mi) / Si;

        sum_x  += 1.0 / idx;
        sum_y  += y;
        sum_xx += (1.0 / idx) * (1.0 / idx);
        sum_xy += (y) * (1.0 / idx);
    }

    double denom = (n * sum_xx) - (sum_x * sum_x);
    if (fabs(denom) < 1e-9) {
        result->R_inf = 1.0;
        result->A = 0.0;
        result->R_inf_std_err = 0.0;
        result->A_std_err = 0.0;
        return;
    }

    double a = ((n * sum_xy) - (sum_x * sum_y)) / denom;
    double b = ((sum_y * sum_xx) - (sum_x * sum_xy)) / denom;

    result->R_inf = b;
    result->A = -a;

    // --- Расчёт стандартных ошибок оценок ---
    double sum_sq_residuals = 0.0;
    for (size_t i = 0; i < model->step_count; ++i) {
        double Si = (double)model->S[i];
        double mi = (double)model->m[i];
        double idx = (double)(i + 1);

        double y = (Si - mi) / Si;
        double y_fit = b - (a / idx);

        double residual = y - y_fit;
        sum_sq_residuals += residual * residual;
    }

    double s2 = sum_sq_residuals / (n - 2.0); // оценка дисперсии ошибок
    result->R_inf_std_err = sqrt(s2 * sum_xx / denom);
    result->A_std_err     = sqrt(s2 * n / denom);
}

double lapadula_reliability(double R_inf, double A, uint32_t i) {
    if (i == 0) return R_inf;
    return R_inf - (A / (double)i);
}
