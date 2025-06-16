#include "data_property_checker.h"


#include "tools/tools_config.h"

#ifdef ERROR_MONITOR_ENABLE
#include "error_monitor/error_monitor.h"
#endif

uint8_t data_property_check_single(const property_checker_t* checker, const void* structure) {
    if (!checker || !checker->check_function) {
        return  0;
    }

    uint8_t ok = checker->check_function(structure);
    if (!ok) {

#ifdef ERROR_MONITOR_ENABLE
        error_monitor_save_event(
            __FILE__,
            checker->property_name,
            "Property check failed",
            __LINE__,
            ERROR_LEVEL_ERROR
        );
#endif

    }
    return ok;
}

uint8_t data_property_check_all(const property_checker_t* checkers, uint32_t count, const void* structure) {
    if (!checkers || count == 0) {
        return 0;
    }

    uint8_t overall_ok = 1;
    for (uint32_t i = 0; i < count; ++i) {
        if (!data_property_check_single(&checkers[i], structure)) {
            overall_ok = 0;
        }
    }
    return overall_ok;
}