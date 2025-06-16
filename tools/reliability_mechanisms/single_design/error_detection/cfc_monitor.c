#include "cfc_monitor.h"


#include "tools/tools_config.h"

#ifdef ERROR_MONITOR_ENABLE
#include "error_monitor/error_monitor.h"
#endif

void cfc_monitor_init(cfc_monitor_t* monitor) {
    monitor->signature = 0;
}

void cfc_monitor_enter(cfc_monitor_t* monitor, uint32_t block_signature) {
    monitor->signature = block_signature;
}

uint8_t cfc_monitor_check(cfc_monitor_t* monitor, uint32_t expected_signature, const char* context) {
    if (monitor->signature != expected_signature) {

#ifdef ERROR_MONITOR_ENABLE
        error_monitor_save_event(
            __FILE__, context,
            "Control Flow Check Failed",
            __LINE__,
            ERROR_LEVEL_CRITICAL
        );
#endif

        return 0;
    }
    return 1;
}
