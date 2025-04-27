#ifndef RELIABILITY_H
#define RELIABILITY_H

#include <stddef.h>

/**
 * Оценить параметры модели Джелински–Моранды.
 * @param t    Массив длины k с интервалами между отказами t[0]=t1 ... t[k-1]=tk.
 * @param k    Количество наблюдений (число интервалов).
 * @param N    Указатель, куда запишется оценка N.
 * @param Cd   Указатель, куда запишется оценка C_D.
 * @return     0 при успехе, отрицательное при ошибке (например, сходимость не достигнута).
 */
int estimate_jm_parameters(const double *t, size_t k, double *N, double *Cd);

/**
 * Среднее время до следующего (k+1)-го отказа:
 * E[t_{k+1}] = 1 / (C_D * (N - (k-1)))
 */
double jm_mean_time_to_next_failure(double Cd, double N, size_t k);

/**
 * Надежность модели (вероятность безотказной работы в течение T после последнего отказа):
 * R(T) = exp(- λ_k * T), где λ_k = C_D * (N - (k-1)).
 */
double jm_reliability(double Cd, double N, size_t k, double T);

#endif /* RELIABILITY_H */
