#include "data_property_checker.h"

#include "error_monitor/error_monitor.h"

bool data_property_check_single(const property_checker_t* checker, const void* structure) {
    if (!checker || !checker->check_function) {
        return false;
    }

    bool ok = checker->check_function(structure);
    if (!ok) {
        error_monitor_save_event(
            __FILE__,
            checker->property_name,
            "Property check failed",
            __LINE__,
            ERROR_LEVEL_ERROR
        );
    }
    return ok;
}

bool data_property_check_all(const property_checker_t* checkers, uint32_t count, const void* structure) {
    if (!checkers || count == 0) {
        return false;
    }

    bool overall_ok = true;
    for (uint32_t i = 0; i < count; ++i) {
        if (!data_property_check_single(&checkers[i], structure)) {
            overall_ok = false;
        }
    }
    return overall_ok;
}