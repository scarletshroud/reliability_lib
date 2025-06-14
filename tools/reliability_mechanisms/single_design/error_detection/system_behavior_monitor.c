#include "system_behavior_monitor.h"
#include "error_monitor/error_monitor.h"      // Для фиксации ошибок

#include "platform/adapter/platform_adapter.h"

void behavior_monitor_init(behavior_monitor_t* monitor, uint32_t timeout_us, const char* context) {
    monitor->last_kick_timestamp_us = platform_timer_get_us();
    monitor->timeout_us = timeout_us;
    monitor->monitored_context = context;
    monitor->active = 0;

    platform_wdt_init(timeout_us);
}

void behavior_monitor_start(behavior_monitor_t* monitor) {
    platform_wdt_start();

    monitor->active = 1;
}

void behavior_monitor_kick(behavior_monitor_t* monitor) {
    if (monitor && monitor->active) {
        monitor->last_kick_timestamp_us = platform_timer_get_us();
        platform_wdt_kick();
    }
}

uint8_t behavior_monitor_check(const behavior_monitor_t* monitor) {
    if (!monitor || !monitor->active) {
        return 0;
    }

    uint32_t now = platform_timer_get_us();
    uint32_t elapsed = now - monitor->last_kick_timestamp_us;

    if (elapsed > monitor->timeout_us) {
        error_monitor_save_event(
            __FILE__,
            monitor->monitored_context,
            "Behavior monitor timeout",
            __LINE__,
            ERROR_LEVEL_CRITICAL
        );
        return 0;
    }

    return 1;
}