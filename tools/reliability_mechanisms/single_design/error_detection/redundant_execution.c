#include "redundant_execution.h"

#include "tools/tools_config.h"
#include "error_monitor/error_monitor.h"

uint8_t redundant_execute_noargs(
    void (*target_fn)(void* result),
    redundant_compare_fn compare,
    void* result_buffer1,
    void* result_buffer2,
    const char* context
) {
    if (!target_fn || !compare || !result_buffer1 || !result_buffer2) {
        return 0;
    }

    target_fn(result_buffer1);
    target_fn(result_buffer2);

    if (!compare(result_buffer1, result_buffer2)) {

#ifdef ERROR_MONITOR_ENABLE
        error_monitor_save_event(__FILE__, context, "Redundant execution mismatch", __LINE__, ERROR_LEVEL_ERROR);
#endif

        return 0;
    }

    return 1;
}

uint8_t redundant_execute_with_args(
    void (*target_fn)(void* result, const void* arg),
    redundant_compare_fn compare,
    const void* input_arg,
    void* result_buffer1,
    void* result_buffer2,
    const char* context
) {
    if (!target_fn || !compare || !input_arg || !result_buffer1 || !result_buffer2) {
        return 0;
    }

    target_fn(result_buffer1, input_arg);
    target_fn(result_buffer2, input_arg);

    if (!compare(result_buffer1, result_buffer2)) {

#ifdef ERROR_MONITOR_ENABLE
        error_monitor_save_event(__FILE__, context, "Redundant execution mismatch", __LINE__, ERROR_LEVEL_ERROR);
#endif

        return 0;
    }

    return 1;
}