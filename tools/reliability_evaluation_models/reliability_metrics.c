#include "reliability_metrics.h"

#define MAX_FAILURES_TRACKED 256

static uint32_t failure_counter = 0;
static uint32_t failure_timestamps[MAX_FAILURES_TRACKED];

void reliability_metrics_reset(void) {
    failure_counter = 0;
}

void reliability_metrics_register_failure(uint32_t timestamp) {
    if (failure_counter < MAX_FAILURES_TRACKED) {
        failure_timestamps[failure_counter++] = timestamp;
    }
}

/// Упрощённый расчёт MTBF и других показателей
reability_metrics_t reliability_metrics_calculate(uint32_t current_time_sec, uint32_t reliability_interval_sec) {
    reability_metrics_t stats = { 0 };
    stats.failures = failure_counter;

    if (failure_counter == 0 || current_time_sec == 0) {
        stats.mtbf = -1.0f;
        stats.lambda = 0.0f;
        stats.reliability_q = 1.0f;
        stats.observed_time_sec = current_time_sec;
        return stats;
    }

    // Среднее время между сбоями
    stats.mtbf = (float)(current_time_sec) / failure_counter;

    // Интенсивность отказов (λ)
    stats.lambda = 1.0f / stats.mtbf;

    // Вероятность безотказной работы на интервале reliability_interval_sec:
    stats.reliability_q = expf(-stats.lambda * reliability_interval_sec);

    stats.observed_time_sec = current_time_sec;
    return stats;
}
