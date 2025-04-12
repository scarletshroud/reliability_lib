#ifndef REABILITY_METRICS_H_
#define REABILITY_METRICS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * doxygen
     *
     */

     // todo Можно добавить среднюю наработку на отказ
     // todo Можно оценивать показель надежности ПО: средняя тяжесть ошибок

    typedef struct {
        float mtbf;           // Среднее время между отказами
        float lambda;         // Интенсивность отказов
        float reliability_q;  // Вероятность безотказной работы на интервале t
        uint32_t failures;    // Количество отказов
        uint32_t observed_time_sec; // Общее время наблюдения
    } reability_metrics_t;

    /// Инициализация расчёта
    void reliability_metrics_reset(void);

    /// Обновление данных о сбоях (например, из error_collector)
    void reliability_metrics_register_failure(uint32_t timestamp);

    /// Расчёт метрик (на интервале от 0 до now_time_sec)
    reability_metrics_t reliability_metrics_calculate(uint32_t now_time_sec, uint32_t reliability_interval_sec);

#ifdef __cplusplus
}
#endif

#endif