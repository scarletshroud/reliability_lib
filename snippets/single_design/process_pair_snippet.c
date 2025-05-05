#include "process_pair_snippet.h"

#include "tools/reliability_mechanisms/single_design/process_pair.h"

// Целевая функция
bool calculate_sum(void* result, const void* input) {
    const uint8_t* arr = (const uint8_t*)input;
    uint32_t* sum = (uint32_t*)result;
    *sum = 0;
    for (int i = 0; i < 5; ++i) {
        *sum += arr[i];
    }
    return true;
}

bool compare_uint32(const void* a, const void* b) {
    return (*(const uint32_t*)a == *(const uint32_t*)b);
}

void process_pair_snippet() {
    uint8_t input_data[5] = { 1, 2, 3, 4, 5 };
    uint32_t result_main = 0, result_backup = 0;

    process_pair_t pair = {
        .active = calculate_sum,
        .backup = calculate_sum,
        .compare = compare_uint32,
        .mode = PROCESS_PAIR_ACTIVE_ACTIVE,
        .context = __func__
    };

    bool ok = process_pair_execute(&pair, input_data, &result_main, &result_backup);

    if (ok) {
        printf("Process pair success. Result = %u\n", result_main);
    } else {
        printf("Process pair failure.\n");
    }
}