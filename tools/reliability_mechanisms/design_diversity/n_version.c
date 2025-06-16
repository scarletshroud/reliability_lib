#include "n_version.h"

#include "tools/tools_config.h"

#ifdef ERROR_MONITOR_ENABLE
#include "error_monitor/error_monitor.h"
#endif

bool n_version_execute(
    const n_version_exec_t* exec,
    const void* input,
    void** result_buffers,
    void* selected_output
) {
    if (!exec || exec->version_count < 2 || exec->version_count > MAX_N_VERSIONS || !exec->compare || !exec->copy) {
        return false;
    }

    // 1. Выполнить все версии
    for (uint32_t i = 0; i < exec->version_count; ++i) {
        if (!exec->versions[i] || !result_buffers[i]) return false;

        bool ok = exec->versions[i](result_buffers[i], input);
        if (!ok) {

#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, exec->context, "Version execution failed", __LINE__, ERROR_LEVEL_ERROR);
#endif

            return false;
        }
    }

    // 2. Подсчёт голосов
    uint32_t votes[MAX_N_VERSIONS] = { 0 };

    for (uint32_t i = 0; i < exec->version_count; ++i) {
        for (uint32_t j = 0; j < exec->version_count; ++j) {
            if (exec->compare(result_buffers[i], result_buffers[j])) {
                votes[i]++;
            }
        }
    }

    // 3. Обработка по режиму
    switch (exec->mode) {

        case N_VERSION_VOTING_STRICT:
            for (uint32_t i = 1; i < exec->version_count; ++i) {
                if (!exec->compare(result_buffers[0], result_buffers[i])) {

#ifdef ERROR_MONITOR_ENABLE
                    error_monitor_save_event(__FILE__, exec->context, "Strict voting mismatch", __LINE__, ERROR_LEVEL_ERROR);
#endif 

                    return false;
                }
            }
            exec->copy(selected_output, result_buffers[0]);
            return true;

        case N_VERSION_VOTING_MAJORITY:
        {
            uint32_t majority_threshold = exec->version_count / 2 + 1;
            for (uint32_t i = 0; i < exec->version_count; ++i) {
                if (votes[i] >= majority_threshold) {
                    exec->copy(selected_output, result_buffers[i]);
                    return true;
                }
            }

#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, exec->context, "Majority voting failed", __LINE__, ERROR_LEVEL_ERROR);
#endif 

            return false;
        }

        case N_VERSION_VOTING_THRESHOLD:
        {
            if (exec->threshold == 0 || exec->threshold > exec->version_count) {

#ifdef ERROR_MONITOR_ENABLE
                error_monitor_save_event(__FILE__, exec->context, "Invalid threshold", __LINE__, ERROR_LEVEL_ERROR);
#endif

                return false;
            }

            for (uint32_t i = 0; i < exec->version_count; ++i) {
                if (votes[i] >= exec->threshold) {
                    exec->copy(selected_output, result_buffers[i]);
                    return true;
                }
            }

#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, exec->context, "Threshold voting failed", __LINE__, ERROR_LEVEL_ERROR);
#endif

            return false;
        }

        default:

#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, exec->context, "Unknown voting mode", __LINE__, ERROR_LEVEL_ERROR);
#endif

            return false;
    }
}