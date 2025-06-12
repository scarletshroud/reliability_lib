#ifndef DEADLINE_MONITOR_H_
#define DEADLINE_MONITOR_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
    /**
    *
    */
    typedef struct {
        uint32_t start_time;  // метка начала отсчета
        uint32_t stop_time;   // метка окончания отсчета
        uint32_t deadline_us; // допустимое время выполнения в микросекундах
        bool     active;

        /* Statistics */
        bool timing_violation;
        uint32_t count;
        uint64_t total_time_us;
        uint32_t max_time_us;
        uint32_t min_time_us;
    } deadline_monitor_t;

    /**
     *
     */
    void deadline_monitor_init(deadline_monitor_t* monitor, uint32_t deadline_us);

    /**
     *
     */
    void deadline_monitor_start(deadline_monitor_t* monitor);

    /**
     *
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