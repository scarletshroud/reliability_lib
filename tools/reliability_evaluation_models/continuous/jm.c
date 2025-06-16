#include "jm.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define JM_TOL     1e-6   /* Допустимая точность по N */
#define JM_MAX_IT  100    /* Максимум итераций бищекции */

/* Вспомогательная функция: f(N) = (k-1)*Σ t_i - Σ (N - i +1) t_i * Σ (1/(N - i +1)) */
static double jm_f(double N, const double *t, size_t k) {
    size_t i;
    double sum_t = 0.0, sum_wt = 0.0, sum_r = 0.0;
    /* i от 1 до k-1, в C — индексы 0..k-2 */
    for (i = 0; i + 1 < k; ++i) {
        double ti = t[i];
        double wi = N - (double)i;        /* N - (i-1) в формуле */
        sum_t  += ti;
        sum_wt += wi * ti;
        sum_r  += 1.0 / wi;
    }
    return (double)(k - 1) * sum_t - sum_wt * sum_r;
}

int estimate_jm_parameters(const double *t, size_t k, double *outN, double *outCd) {
    if (k < 2 || !t || !outN || !outCd) return -1;

    /* 1) Найти N из уравнения f(N)=0 методом бищекции */
    double low = (double)k - 1.0 + JM_TOL;  /* чуть больше k-1 */
    double high = low + 1.0;
    double f_low = jm_f(low, t, k);
    double f_high = jm_f(high, t, k);

    /* Подбор high, чтобы f(high)>0 */
    int iter = 0;
    while (f_high < 0.0 && iter++ < JM_MAX_IT) {
        high *= 2.0;
        f_high = jm_f(high, t, k);
    }
    if (f_low * f_high > 0.0) {
        /* Не удалось найти разного знака на концах отрезка */
        return -2;
    }

    /* Бищекция */
    for (iter = 0; iter < JM_MAX_IT; ++iter) {
        double mid = 0.5 * (low + high);
        double f_mid = jm_f(mid, t, k);
        if (fabs(f_mid) < JM_TOL) {
            *outN = mid;
            break;
        }
        if (f_low * f_mid < 0.0) {
            high = mid;
            f_high = f_mid;
        } else {
            low = mid;
            f_low = f_mid;
        }
        *outN = mid;
    }

    /* 2) Вычислить C_D по формуле (1.7): Σ (1/(N - i +1)) / Σ t_i */
    {
        double sum_r = 0.0, sum_t = 0.0;
        size_t i;
        for (i = 0; i + 1 < k; ++i) {
            sum_t += t[i];
            sum_r += 1.0 / ( (*outN) - (double)i );
        }
        *outCd = sum_r / sum_t;
    }

    return 0;
}

double jm_mean_time_to_next_failure(double Cd, double N, size_t k) {
    /* λ_k = C_D * (N - (k-1)) */
    double lambda_k = Cd * (N - (double)(k - 1));
    return 1.0 / lambda_k;
}

double jm_reliability(double Cd, double N, size_t k, double T) {
    double lambda_k = Cd * (N - (double)(k - 1));
    return exp(-lambda_k * T);
}

/* Пример использования */
#ifdef JM_TEST
int jm_main(void) {
    /* Пусть у нас k=5 и интервалы t1..t5: */
    double t[] = { 10.5, 12.3, 9.7, 11.8, 10.1 };
    size_t k = sizeof(t)/sizeof(*t);
    double N, Cd;

    if (estimate_jm_parameters(t, k, &N, &Cd) == 0) {
        printf("Оценка N  = %.5f\n", N);
        printf("Оценка C_D = %.8f\n", Cd);
        printf("E[t_{6}] = %.3f\n", jm_mean_time_to_next_failure(Cd, N, k));
        printf("R(5.0)   = %.6f\n", jm_reliability(Cd, N, k, 5.0));
    } else {
        fprintf(stderr, "Ошибка оценки параметров модели\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
#endif
