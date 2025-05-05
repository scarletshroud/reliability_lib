#include "recovery_block.h"
#include "error_monitor/error_monitor.h"

bool recovery_block_execute(
    const recovery_block_t* rb,
    const void* input,
    void* output
) {
    if (!rb || rb->variant_count == 0 || !output) return false;

    if (rb->mode == RB_MODE_SEQUENTIAL) {
        // Обычный RB
        for (uint32_t i = 0; i < rb->variant_count; ++i) {
            if (!rb->variants[i]) continue;

            bool ok = rb->variants[i](output, input);
            if (!ok) {
                error_monitor_save_event(__FILE__, rb->context, "RB variant failed", __LINE__, ERROR_LEVEL_WARNING);
                continue;
            }

            if (rb->acceptance_test && rb->acceptance_test(output)) {
                return true;
            } else {
                error_monitor_save_event(__FILE__, rb->context, "RB acceptance test failed", __LINE__, ERROR_LEVEL_WARNING);
            }
        }

        error_monitor_save_event(__FILE__, rb->context, "RB: all variants failed", __LINE__, ERROR_LEVEL_ERROR);
        return false;

    } else if (rb->mode == RB_MODE_CONSENSUS) {
        // Consensus RB
        if (!rb->consensus_compare || !rb->consensus_copy) return false;

        void* temp_results[MAX_RB_VARIANTS] = { 0 };
        uint8_t local_buffers[MAX_RB_VARIANTS][64]; // небольшой буфер (например, до 64 байт результата)

        for (uint32_t i = 0; i < rb->variant_count; ++i)
            temp_results[i] = local_buffers[i];

        // Выполнить все варианты
        for (uint32_t i = 0; i < rb->variant_count; ++i) {
            if (!rb->variants[i] || !temp_results[i]) continue;
            bool ok = rb->variants[i](temp_results[i], input);
            if (!ok) {
                error_monitor_save_event(__FILE__, rb->context, "CRB variant execution failed", __LINE__, ERROR_LEVEL_WARNING);
                return false;
            }
        }

        // Голосование: ищем большинство
        uint32_t votes[MAX_RB_VARIANTS] = { 0 };

        for (uint32_t i = 0; i < rb->variant_count; ++i) {
            for (uint32_t j = 0; j < rb->variant_count; ++j) {
                if (rb->consensus_compare(temp_results[i], temp_results[j])) {
                    votes[i]++;
                }
            }
        }

        // Выбор наибольшего голосования
        uint32_t winner = 0;
        uint32_t max_votes = votes[0];

        for (uint32_t i = 1; i < rb->variant_count; ++i) {
            if (votes[i] > max_votes) {
                winner = i;
                max_votes = votes[i];
            }
        }

        if (max_votes < (rb->variant_count / 2 + 1)) {
            error_monitor_save_event(__FILE__, rb->context, "CRB: no consensus", __LINE__, ERROR_LEVEL_ERROR);
            return false;
        }

        rb->consensus_copy(output, temp_results[winner]);
        return true;
    }

    return false;
}


/*

bool primary_sum(void* result, const void* input) {
    const uint8_t* arr = (const uint8_t*)input;
    uint32_t* sum = (uint32_t*)result;
    *sum = 0;
    for (int i = 0; i < 5; ++i)
        *sum += arr[i];
    return true;
}

bool backup_sum(void* result, const void* input) {
    const uint8_t* arr = (const uint8_t*)input;
    uint32_t* sum = (uint32_t*)result;
    *sum = arr[0] * 5;  // простая оценка
    return true;
}

bool test_nonzero(const void* result) {
    return (*(const uint32_t*)result > 0);
}

void example_recovery_block() {
    uint8_t input[] = {0, 0, 0, 0, 0}; // приведёт к отказу основного варианта
    uint32_t result = 0;

    recovery_block_t rb = {
        .variants = { primary_sum, backup_sum },
        .variant_count = 2,
        .acceptance_test = test_nonzero,
        .context = __func__
    };

    bool ok = recovery_block_execute(&rb, input, &result);

    if (ok) {
        printf("RecoveryBlock: Success, result = %u\n", result);
    } else {
        printf("RecoveryBlock: All variants failed\n");
    }
}

bool v1(void* result, const void* input) {
    *(uint32_t*)result = (*(const uint8_t*)input) * 2;
    return true;
}
bool v2(void* result, const void* input) {
    *(uint32_t*)result = (*(const uint8_t*)input) * 2;
    return true;
}
bool v3_bad(void* result, const void* input) {
    *(uint32_t*)result = (*(const uint8_t*)input) + 1000;
    return true;
}

bool cmp_u32(const void* a, const void* b) {
    return (*(const uint32_t*)a) == (*(const uint32_t*)b);
}
void copy_u32(void* dest, const void* src) {
    *(uint32_t*)dest = *(const uint32_t*)src;
}

void example_crb() {
    uint8_t input = 10;
    uint32_t output = 0;

    recovery_block_t crb = {
        .variants = {v1, v2, v3_bad},
        .variant_count = 3,
        .mode = RB_MODE_CONSENSUS,
        .consensus_compare = cmp_u32,
        .consensus_copy = copy_u32,
        .context = __func__
    };

    if (recovery_block_execute(&crb, &input, &output)) {
        printf("CRB Success. Output = %u\n", output);
    } else {
        printf("CRB Failed\n");
    }
}


*/