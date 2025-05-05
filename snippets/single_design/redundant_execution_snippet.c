#include "cfc_monitor_snippet.h"

#include "tools/reliability_mechanisms/single_design/error_detection/redundant_execution.h"

// Пример вычисляемой функции
void compute_checksum(void* result, const void* input) {
    const uint8_t* data = (const uint8_t*)input;
    uint32_t* checksum = (uint32_t*)result;
    *checksum = 0;
    for (uint8_t i = 0; i < 10; ++i) {
        *checksum += data[i];
    }
}

// Пример функции сравнения
bool compare_uint32(const void* a, const void* b) {
    return (*(const uint32_t*)a) == (*(const uint32_t*)b);
}

void redundant_execution_snippet() {
    uint8_t input_data[10] = { 1,2,3,4,5,6,7,8,9,10 };
    uint32_t result1 = 0;
    uint32_t result2 = 0;

    bool ok = redundant_execute_with_args(
        compute_checksum,
        compare_uint32,
        input_data,
        &result1,
        &result2,
        __func__
    );

    if (!ok) {

    } else {

    }
}