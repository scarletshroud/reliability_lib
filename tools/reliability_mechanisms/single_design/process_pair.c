#include "process_pair.h"

#include "error_monitor/error_monitor.h"

bool process_pair_execute(
    const process_pair_t* pair,
    const void* input,
    void* result_main,
    void* result_backup
) {
    if (!pair || !pair->active || !pair->backup || !pair->compare) return false;

    bool main_ok = pair->active(result_main, input);

    if (pair->mode == PROCESS_PAIR_ACTIVE_PASSIVE) {
        if (!main_ok) {
            error_monitor_save_event(__FILE__, pair->context, "Active failed, switching to passive", __LINE__, ERROR_LEVEL_WARNING);
            bool backup_ok = pair->backup(result_backup, input);
            if (!backup_ok) {
                error_monitor_save_event(__FILE__, pair->context, "Passive also failed", __LINE__, ERROR_LEVEL_ERROR);
                return false;
            }
            return true;
        }
        return true;
    }

    if (pair->mode == PROCESS_PAIR_ACTIVE_ACTIVE) {
        bool backup_ok = pair->backup(result_backup, input);
        if (!main_ok || !backup_ok) {
            error_monitor_save_event(__FILE__, pair->context, "One of active processes failed", __LINE__, ERROR_LEVEL_ERROR);
            return false;
        }

        if (!pair->compare(result_main, result_backup)) {
            error_monitor_save_event(__FILE__, pair->context, "Mismatch between active-active results", __LINE__, ERROR_LEVEL_ERROR);
            return false;
        }
        return true;
    }

    return false;
}