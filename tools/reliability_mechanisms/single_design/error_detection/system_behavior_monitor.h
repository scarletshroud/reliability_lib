#ifndef SYSTEM_BEHAVIOR_MONITOR_H
#define SYSTEM_BEHAVIOR_MONITOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        uint32_t last_kick_timestamp_us;
        uint32_t timeout_us;
        const char* monitored_context;
        bool active;
    } behavior_monitor_t;

    /// Инициализация монитора поведения
    void behavior_monitor_init(behavior_monitor_t* monitor, uint32_t timeout_us, const char* context);

    /// Подача признака активности (kick)
    void behavior_monitor_kick(behavior_monitor_t* monitor);

    /// Проверка срабатывания таймаута
    void behavior_monitor_check(const behavior_monitor_t* monitor);

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_BEHAVIOR_MONITOR_H
