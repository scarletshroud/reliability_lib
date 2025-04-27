#ifndef MUSA_H
#define MUSA_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Вычислить коэффициент уменьшения числа ошибок:
 *   B = n_found / m_failures
 * где n_found — число найденных ошибок,
 *       m_failures — число зафиксированных отказов.
 */
double musa_compute_B(unsigned int n_found, unsigned int m_failures);

/**
 * Вычислить первоначальное число ошибок в ПО:
 *   N0 = B * M0
 * где M0 — общее число отказов за весь жизненный цикл.
 */
double musa_compute_N0(double B, unsigned int M0);

/**
 * Средняя наработка на отказ (MTTF) по модели Муса:
 *   T_cp = T0 * exp(c * τ / (M0 * T0))
 * где
 *   T0  — начальная наработка на отказ,
 *   c   — коэффициент «сжатия» (обычно = время тестирования τ),
 *   τ   — время тестирования,
 *   M0  — общее число отказов за полный цикл.
 */
double musa_mean_time_to_failure(double T0, double c, double tau, unsigned int M0);

/**
 * Среднее число отказов m(τ) за время τ:
 *   m = M0 * (1 − exp(−c * τ / (M0 * T0)))
 */
double musa_expected_failures(double M0, double c, double tau, double T0);

/**
 * Интенсивность отказов в конце тестирования (τ):
 *   λ(τ) = (c / T0) * exp(−c * τ / (M0 * T0))
 */
double musa_failure_intensity_end(double c, double tau, unsigned int M0, double T0);

/**
 * Надёжность (вероятность безотказной работы) на период t при постоянной λ:
 *   R(t) = exp(−λ * t)
 */
double musa_reliability(double lambda, double t);

#ifdef __cplusplus
}
#endif

#endif // MUSA_H