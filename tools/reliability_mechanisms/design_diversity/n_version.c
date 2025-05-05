#include "n_version.h"
#include "error_monitor/error_monitor.h"

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
            error_monitor_save_event(__FILE__, exec->context, "Version execution failed", __LINE__, ERROR_LEVEL_ERROR);
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
                    error_monitor_save_event(__FILE__, exec->context, "Strict voting mismatch", __LINE__, ERROR_LEVEL_ERROR);
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
            error_monitor_save_event(__FILE__, exec->context, "Majority voting failed", __LINE__, ERROR_LEVEL_ERROR);
            return false;
        }

        case N_VERSION_VOTING_THRESHOLD:
        {
            if (exec->threshold == 0 || exec->threshold > exec->version_count) {
                error_monitor_save_event(__FILE__, exec->context, "Invalid threshold", __LINE__, ERROR_LEVEL_ERROR);
                return false;
            }

            for (uint32_t i = 0; i < exec->version_count; ++i) {
                if (votes[i] >= exec->threshold) {
                    exec->copy(selected_output, result_buffers[i]);
                    return true;
                }
            }

            error_monitor_save_event(__FILE__, exec->context, "Threshold voting failed", __LINE__, ERROR_LEVEL_ERROR);
            return false;
        }

        default:
            error_monitor_save_event(__FILE__, exec->context, "Unknown voting mode", __LINE__, ERROR_LEVEL_ERROR);
            return false;
    }
}


/*
// Пример функции-версии
bool version1(void* result, const void* input) {
    *(uint32_t*)result = (*(const uint8_t*)input) + 1;
    return true;
}

bool version2(void* result, const void* input) {
    *(uint32_t*)result = (*(const uint8_t*)input) + 1;
    return true;
}

bool version3_bugged(void* result, const void* input) {
    *(uint32_t*)result = (*(const uint8_t*)input) + 2; // ошибка!
    return true;
}

bool compare_u32(const void* a, const void* b) {
    return (*(const uint32_t*)a) == (*(const uint32_t*)b);
}

void copy_u32(void* dst, const void* src) {
    *(uint32_t*)dst = *(const uint32_t*)src;
}

void example_n_version_exec() {
    uint8_t input = 5;
    uint32_t res1, res2, res3, output;

    void* buffers[3] = { &res1, &res2, &res3 };

    n_version_exec_t exec = {
        .versions = {version1, version2, version3_bugged},
        .version_count = 3,
        .compare = compare_u32,
        .copy = copy_u32,
        .context = __func__
    };

    bool ok = n_version_execute(&exec, &input, buffers, &output);

    if (ok) {
        printf("Selected result (by majority): %u\n", output);
    } else {
        printf("N-Version Execution Failed.\n");
    }
}


t / (1 - n)
void example_t_out_of_n() {
    uint8_t input = 7;
    uint32_t res1, res2, res3, res4, output;
    void* buffers[4] = {&res1, &res2, &res3, &res4};

    // 4 реализации, из которых 3 верные
    n_version_exec_t exec = {
        .versions = {v1, v2, v3, v4_bugged},
        .version_count = 4,
        .threshold = 3, // требуем минимум 3 совпадения
        .compare = compare_u32,
        .copy = copy_u32,
        .mode = N_VERSION_VOTING_THRESHOLD,
        .context = __func__
    };

    bool ok = n_version_execute(&exec, &input, buffers, &output);
    if (ok) {
        printf("t/(n-1) success: %u\n", output);
    } else {
        printf("t/(n-1) failed\n");
    }
}



*/