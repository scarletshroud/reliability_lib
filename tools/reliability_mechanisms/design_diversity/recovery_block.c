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
