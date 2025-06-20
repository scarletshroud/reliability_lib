#include "deadline_monitor.h"

#include "tools/tools_config.h"

#ifdef ERROR_MONITOR_ENABLE
#include "error_monitor/error_monitor.h"
#endif

#include "platform/adapter/platform_adapter.h"

void deadline_monitor_init(deadline_monitor_t* monitor, uint32_t deadline_us) {
    monitor->start_time = 0;
    monitor->stop_time = 0;
    monitor->deadline_us = deadline_us;
    monitor->active = 0;
    monitor->timing_violation = 0;
}

void deadline_monitor_start(deadline_monitor_t* monitor) {
    monitor->start_time = platform_timer_get_us();
    monitor->active = 1;
}

void deadline_monitor_stop(deadline_monitor_t* monitor, const char* context) {
    if (!monitor->active) {
        return;
    }

    monitor->stop_time = platform_timer_get_us();
    monitor->stop_time = get_time_us();
    monitor->active = 0;

    uint32_t elapsed = monitor->stop_time - monitor->start_time;

    if (elapsed > monitor->deadline_us) {
        monitor->timing_violation = 1;

#ifdef ERROR_MONITOR_ENABLE
        error_monitor_save_event(__FILE__, context, "Deadline violation", __LINE__, ERROR_LEVEL_ERROR);
#endif

    }

    monitor->total_time_us += elapsed;
    monitor->count++;

    if (elapsed > monitor->max_time_us) {
        monitor->max_time_us = elapsed;
    }
    if (elapsed < monitor->min_time_us) {
        monitor->min_time_us = elapsed;
    }
}

uint32_t deadline_monitor_get_average(const deadline_monitor_t* monitor) {
    if (monitor->count == 0) {
        return 0;
    }
    return (uint32_t)(monitor->total_time_us / monitor->count);
}

uint32_t deadline_monitor_get_min(const deadline_monitor_t* monitor) {
    return (monitor->count == 0) ? 0 : monitor->min_time_us;
}

uint32_t deadline_monitor_get_max(const deadline_monitor_t* monitor) {
    return (monitor->count == 0) ? 0 : monitor->max_time_us;
}