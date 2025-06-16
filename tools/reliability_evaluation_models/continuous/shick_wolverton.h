#ifndef SHICK_WOLVERTON_H
#define SHICK_WOLVERTON_H

#include <stddef.h>

/**
 * Оценить параметры модели Шика–Волвертона.
 * @param t     Массив длины k с интервалами между отказами t[0]=t1 ... t[k-1]=tk.
 * @param k     Количество наблюдений (число интервалов).
 * @param outN  Указатель, куда запишется оценка N.
 * @param outC  Указатель, куда запишется оценка C.
 * @return      0 при успехе, отрицательное при ошибке (например, неудачно подобрали границы).
 */
int estimate_sw_parameters(const double *t, size_t k, double *outN, double *outC);

/**
 * Среднее время до следующего отказа (аналогично JM):
 * E[t_{k+1}] ≈ 1 / λ_k, где
 *   λ_k = C * (N - (k-1)) * (T_{k-1} + t_k/2).
 */
double sw_mean_time_to_next_failure(double C, double N, size_t k);

/**
 * Надежность в течение T после последнего отказа:
 * R(T) = exp(- λ_k * T).
 */
double sw_reliability(double C, double N, size_t k, double T);

#endif /* RELIABILITY_SW_H */