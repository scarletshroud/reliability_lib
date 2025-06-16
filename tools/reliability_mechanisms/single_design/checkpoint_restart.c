#include "checkpoint_restart.h"

#include <string.h>

#include "tools/tools_config.h"
#include "error_monitor/error_monitor.h"

uint8_t checkpoint_static_restart(checkpoint_t* checkpoint, void* current_state) {
    if (!checkpoint || !checkpoint->state_buffer || !current_state || checkpoint->size == 0) {
        return 0;
    }

    memcpy(current_state, checkpoint->state_buffer, checkpoint->size);

#ifdef ERROR_MONITOR_ENABLE
    error_monitor_save_event(__FILE__, "Static Restart", "Restored static checkpoint", __LINE__, ERROR_LEVEL_WARNING);
#endif

    return 1;
}

uint8_t checkpoint_dynamic_save(checkpoint_t* checkpoint, const void* state) {
    if (!checkpoint || !checkpoint->state_buffer || !state || checkpoint->size == 0) {
        return 0;
    }

    memcpy(checkpoint->state_buffer, state, checkpoint->size);
    return 1;
}

uint8_t checkpoint_dynamic_restore(const checkpoint_t* checkpoint, void* target) {
    if (!checkpoint || !checkpoint->state_buffer || !target || checkpoint->size == 0) {
        return 0;
    }

    memcpy(target, checkpoint->state_buffer, checkpoint->size);

#ifdef ERROR_MONITOR_ENABLE
    error_monitor_save_event(__FILE__, "Dynamic Restart", "Restored dynamic checkpoint", __LINE__, ERROR_LEVEL_WARNING);
#endif

    return 1;
}