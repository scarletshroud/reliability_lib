// shuman_reliability.h
#ifndef SHUMAN_RELIABILITY_H
#define SHUMAN_RELIABILITY_H

#include <stddef.h>

// Исходные данные для модели Шумана
typedef struct {
    // массив длительностей прогонов (часы)
    const double *t_intervals;
    // массив числа ошибок в каждом прогоне
    const size_t *errors;
    // число прогонов (длина массивов выше)
    size_t runs;
    // общее число машинных команд на этапе тестирования
    double It;

    // контрольная точка A:
    //   Ec_tA — накопленное число ошибок к tA
    //   chi_tA — измеренная интенсивность на момент tA
    double tA, Ec_tA, chi_tA;

    // контрольная точка B:
    double tB, Ec_tB, chi_tB;
} ShumanInput;

// Результаты расчётов модели
typedef struct {
    double chi;   // интенсивность появления ошибок
    double Et;    // оценка начального числа ошибок
    double C;     // коэффициент пропорциональности
    double R;     // надёжность на следующий интервал
} ShumanResult;

// Базовые функции (как раньше)
double calc_intensity(const double *t_intervals, const size_t *errors, size_t r);
double estimate_Et(double chi_tA, double Ec_tA, double chi_tB, double Ec_tB);
double estimate_C(double chi_tA, double Ec_tA, double Et, double It);
double reliability(double C, double Et, double It, double Ec_t1, double t);

// «Удобная» обёртка, заполняет ShumanResult по ShumanInput + длине следующего интервала next_t
void shuman_compute(const ShumanInput *in, ShumanResult *out, double next_t);

#endif // SHUMAN_RELIABILITY_H
