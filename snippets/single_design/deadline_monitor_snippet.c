#include "deadline_monitor_snippet.h"

#include "tools/reliability_mechanisms/single_design/error_detection/deadline_monitor.h"

deadline_monitor_t monitor;

void deadline_monitor_snippet(void) {
    deadline_monitor_init(&monitor, 500); // 500 мкс допустимое время
    deadline_monitor_start(&monitor);

    // --- Критический код ---
    do_something();

    deadline_monitor_stop(&monitor, __func__);

    uint32_t avg = deadline_monitor_get_average(&monitor);
    uint32_t min = deadline_monitor_get_min(&monitor);
    uint32_t max = deadline_monitor_get_max(&monitor);

    printf("Execution: avg = %u us, min = %u us, max = %u us\n", avg, min, max);
}