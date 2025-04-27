// shuman_reliability.c
#include "shuman.h"
#include <math.h>
#include <stddef.h>

double calc_intensity(const double *t_intervals, const size_t *errors, size_t r) {
    double t1 = 0.0;
    size_t sumA = 0;
    for (size_t i = 0; i < r; ++i) {
        t1     += t_intervals[i];
        sumA   += errors[i];
    }
    return (t1 > 0.0) ? ((double)sumA / t1) : NAN;
}

double estimate_Et(double chi_tA, double Ec_tA, double chi_tB, double Ec_tB) {
    if (chi_tA <= 0.0 || fabs(chi_tB/chi_tA - 1.0) < 1e-12) return NAN;
    double num = chi_tB * Ec_tA - chi_tA * Ec_tB;
    double den = chi_tB/chi_tA - 1.0;
    return num / den;
}

double estimate_C(double chi_tA, double Ec_tA, double Et, double It) {
    double term = Et/It - Ec_tA;
    return (term > 0.0) ? (chi_tA / term) : NAN;
}

double reliability(double C, double Et, double It, double Ec_t1, double t) {
    double expo = C * (Et/It - Ec_t1) * t;
    return exp(expo);
}

void shuman_compute(const ShumanInput *in, ShumanResult *out, double next_t) {
    // 1) интенсивность по всем прогонам
    out->chi = calc_intensity(in->t_intervals, in->errors, in->runs);

    // 2) оценка Et по двум точкам
    out->Et  = estimate_Et(in->chi_tA, in->Ec_tA, in->chi_tB, in->Ec_tB);

    // 3) оценка C
    out->C   = estimate_C(in->chi_tA, in->Ec_tA, out->Et, in->It);

    // 4) надёжность на следующий интервал next_t, начиная от времени tB
    out->R   = reliability(out->C, out->Et, in->It, in->Ec_tB, next_t);
}

/*

 double t_intervals[3] = {100.0, 120.0, 80.0};     // время каждого прогона, часы
    size_t errors[3]     = {12,    15,     10    };   // найдено ошибок
    
    // 1) Оцениваем интенсивность появления ошибок:
    double chi = calc_intensity(t_intervals, errors, 3);
    printf("Интенсивность появления ошибок χ = %.6f ошибок/час\n", chi);
    
    // Допустим, сделали замеры в двух контрольных точках:
    double tA = 150.0;    // часов
    double Ec_tA = 18.0;  // ошибок к этому моменту
    double chi_tA = 0.12; // интенсивность, измеренная в т.A
    
    double tB = 300.0;    // часов
    double Ec_tB = 25.0;  // ошибок
    double chi_tB = 0.0833;// интенсивность в т.B
    
    // 2) Оцениваем начальное число ошибок Et:
    double Et = estimate_Et(chi_tA, Ec_tA, chi_tB, Ec_tB);
    printf("Оценка начального числа ошибок Et = %.3f\n", Et);
    
    // 3) Оцениваем коэффициент C, при том что It = 100000 машинных команд:
    double It = 100000.0;
    double C  = estimate_C(chi_tA, Ec_tA, Et, It);
    printf("Оценка коэффициента C = %.9f\n", C);
    
    // 4) Расчёт надёжности R для следующего часа работы системы после t1 = tB:
    double t = 1.0; // час
    double R = reliability(C, Et, It, Ec_tB, t);
    printf("Надёжность R(%.1f, %.1f) = %.6f\n", tB, t, R);
    
    return 0;

*/