#include <reliability_mechanisms/single_disign/error_detection/cfc_monitor.h>
#include "error_monitor/error_monitor.h"

void cfc_monitor_init(cfc_monitor_t* monitor) {
    monitor->signature = 0;
}

void cfc_monitor_enter(cfc_monitor_t* monitor, uint32_t block_signature) {
    monitor->signature = block_signature;
}

bool cfc_monitor_check(cfc_monitor_t* monitor, uint32_t expected_signature, const char* context) {
    if (monitor->signature != expected_signature) {
        error_monitor_save_event(
            __FILE__, context,
            "Control Flow Check Failed",
            __LINE__,
            ERROR_LEVEL_CRITICAL
        );
        return false;
    }
    return true;
}
