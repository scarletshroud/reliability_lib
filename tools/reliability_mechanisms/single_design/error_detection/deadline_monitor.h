#ifndef DEADLINE_MONITOR_H_
#define DEADLINE_MONITOR_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    /**
    *
    */
    typedef struct {
        uint32_t start_time;    /* Метка начала отсчета */
        uint32_t stop_time;     /* Метка окончания отсчета */
        uint32_t deadline_us;   /* Допустимое время выполнения в микросекундах */
        uint8_t     active;        /* Активность монитора */

        /* Statistics */
        uint8_t timing_violation;  /* Превышение времени */
        uint32_t count;         /* Максимальное время выполнения */
        uint64_t total_time_us; /* Итоговое время выполнения */
        uint32_t max_time_us;   /* Максимальное время выполнения */
        uint32_t min_time_us;   /* Минимальное время выполнения */
    } deadline_monitor_t;

    /**
     * \brief           Функция инициализации монитора проверки временных ограничений
     * \param[in]       monitor: Указатель на монитор
     * \param[in]       dealine_us: Указатель на дедлайн выполнения
    */
    void deadline_monitor_init(deadline_monitor_t* monitor, uint32_t deadline_us);

    /**
     * \brief           Функция запуска монитора проверки временных ограничений
     * \param[in]       monitor: Указатель на монитор
    */
    void deadline_monitor_start(deadline_monitor_t* monitor);

    /**
     * \brief           Функция остановки монитора
     * \param[in]       monitor: Указатель на монитор
     * \param[in]       context: Контекст выполнения
    */
    void deadline_monitor_stop(deadline_monitor_t* monitor, const char* context);

    /// Получить среднее время выполнения операции (в микросекундах)
    uint32_t deadline_monitor_get_average(const deadline_monitor_t* monitor);

    /// Получить минимальное и максимальное время
    uint32_t deadline_monitor_get_min(const deadline_monitor_t* monitor);
    uint32_t deadline_monitor_get_max(const deadline_monitor_t* monitor);

#ifdef __cplusplus
}
#endif

#endif