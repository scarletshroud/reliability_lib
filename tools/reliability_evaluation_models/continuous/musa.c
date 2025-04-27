#include "musa.h"

#include <math.h>

double musa_compute_B(unsigned int n_found, unsigned int m_failures) {
    if (m_failures == 0) return 0.0;
    return (double)n_found / (double)m_failures;
}

double musa_compute_N0(double B, unsigned int M0) {
    return B * (double)M0;
}

double musa_mean_time_to_failure(double T0, double c, double tau, unsigned int M0) {
    if (T0 <= 0.0 || M0 == 0) return 0.0;
    return T0 * exp(c * tau / ((double)M0 * T0));
}

double musa_expected_failures(double M0, double c, double tau, double T0) {
    if (M0 == 0 || T0 <= 0.0) return 0.0;
    return (double)M0 * (1.0 - exp(-c * tau / ((double)M0 * T0)));
}

double musa_failure_intensity_end(double c, double tau, unsigned int M0, double T0) {
    if (T0 <= 0.0 || M0 == 0) return 0.0;
    return (c / T0) * exp(-c * tau / ((double)M0 * T0));
}

double musa_reliability(double lambda, double t) {
    if (lambda < 0.0) return 1.0;
    return exp(-lambda * t);