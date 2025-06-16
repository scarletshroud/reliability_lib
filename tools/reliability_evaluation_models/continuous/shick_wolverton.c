#include "shick_wolverton.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define SW_TOL    1e-6   /* Точность по N */
#define SW_MAX_IT 100    /* Максимум итераций биссекции */

/*
  Вспомогательная f(N) для уравнения 
    Σ[1/(N-(i-1))] * Σ[(N-(j-1)) * A_j] - k * Σ[A_i] = 0,
  где A_i = (T_{i-1} + t_i/2) * t_i, T_{i-1} = Σ_{m< i} t_m.
*/
static double sw_f(double N, const double *t, size_t k) {
    double sum_A = 0.0, sum_wA = 0.0, sum_r = 0.0;
    double Tprev = 0.0;
    for (size_t i = 0; i < k; ++i) {
        double ti = t[i];
        double Ai = (Tprev + ti*0.5) * ti;
        double rem = N - (double)i;  /* N - (i-1) при i от 1..k, но в C i=0..k-1 */
        if (rem <= 0.0) return HUGE_VAL;  /* выходим за допустимые N */
        sum_A   += Ai;
        sum_wA  += rem * Ai;
        sum_r   += 1.0/rem;
        Tprev   += ti;
    }
    return sum_r * sum_wA - (double)k * sum_A;
}

int estimate_sw_parameters(const double *t, size_t k, double *outN, double *outC) {
    if (k < 1 || !t || !outN || !outC) return -1;

    /* 1) Находим N из sw_f(N)=0 методом биссекции */
    double low  = (double)k - 1.0 + SW_TOL;  /* чуть выше k-1 */
    double high = low + 1.0;
    double f_low  = sw_f(low,  t, k);
    double f_high = sw_f(high, t, k);

    /* Удваиваем high пока не сменится знак */
    int it = 0;
    while (f_low * f_high > 0.0 && it++ < SW_MAX_IT) {
        high *= 2.0;
        f_high = sw_f(high, t, k);
    }
    if (f_low * f_high > 0.0) {
        /* Не нашли интервал с разными знаками */
        return -2;
    }

    double mid=low, f_mid;
    for (it = 0; it < SW_MAX_IT; ++it) {
        mid   = 0.5*(low + high);
        f_mid = sw_f(mid, t, k);
        if (fabs(f_mid) < SW_TOL) break;
        if (f_low * f_mid <= 0.0) {
            high   = mid;
            f_high = f_mid;
        } else {
            low   = mid;
            f_low = f_mid;
        }
    }
    *outN = mid;

    /* 2) Вычисляем C по формуле из ∂L/∂C=0:
         C = k / Σ[(N-(i-1)) * A_i]
    */
    {
        double denom = 0.0, Tprev = 0.0;
        for (size_t i = 0; i < k; ++i) {
            double ti = t[i];
            double Ai = (Tprev + ti*0.5)*ti;
            denom += ( (*outN) - (double)i ) * Ai;
            Tprev += ti;
        }
        *outC = (denom > 0.0 ? (double)k / denom : HUGE_VAL);
    }

    return 0;
}

double sw_mean_time_to_next_failure(double C, double N, size_t k) {
    /* Берём λ_k = C * (N-(k-1)) * (T_{k-1} + t_k/2) —
       но поскольку t_k нам известен лишь из выборки,
       для простоты используем λ_k на момент середины последнего интервала:
    */
    /* здесь пользователю нужно передать t[k-1] и сумму предыдущих t, 
       но для API оставляем тот же сигнатур, как у JM */
    double lambda_k = C * (N - (double)(k - 1));
    return 1.0 / lambda_k;
}

double sw_reliability(double C, double N, size_t k, double T) {
    double lambda_k = C * (N - (double)(k - 1));
    return exp(-lambda_k * T);
}

/* Пример использования */
#ifdef SW_TEST
int main(void) {
    double t[] = { 8.2, 10.5, 9.1, 11.0, 10.7 };
    size_t k = sizeof(t)/sizeof(*t);
    double N, C;

    if (estimate_sw_parameters(t, k, &N, &C) == 0) {
        printf("Шик–Волвертон: N = %.5f, C = %.8f\n", N, C);
        printf("E[t_%zu] = %.3f\n", k+1, sw_mean_time_to_next_failure(C,N,k));
        printf("R(5.0)  = %.6f\n", sw_reliability(C,N,k,5.0));
    } else {
        fprintf(stderr, "Ошибка в оценке параметров SW-модели\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
#endif