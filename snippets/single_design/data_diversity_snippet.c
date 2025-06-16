#include "data_diversity_snippet.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "reliability_mechanisms/single_design/error_detection/data_diversity.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 1000
#define INPUT_LEN 16
#define MAX_VARIANTS 5
#define NOISE_LEVEL 4

static int total_runs = 0;
static int injected_faults = 0;
static int detected_errors = 0;
static int true_positives = 0;
static int false_negatives = 0;
static int true_negatives = 0;

// Простая функция обработки: среднее значение байтов
void process_signal(void* result, const void* input) {
    const uint8_t* data = (const uint8_t*)input;
    uint32_t* out = (uint32_t*)result;

    uint32_t sum = 0;
    for (int i = 0; i < INPUT_LEN; ++i) {
        sum += data[i];
    }
    *out = sum / INPUT_LEN;
}

// Сравнение двух результатов
bool compare_uint32(const void* r1, const void* r2) {
    return *(const uint32_t*)r1 == *(const uint32_t*)r2;
}

void run_noise_variation_case(bool inject_fault) {
    uint8_t base_data[INPUT_LEN];
    const void* inputs[MAX_VARIANTS];
    void* outputs[MAX_VARIANTS];
    uint8_t variants[MAX_VARIANTS][INPUT_LEN];
    uint32_t results[MAX_VARIANTS];
    bool injected = false;
    bool success = true;

    for (int i = 0; i < INPUT_LEN; ++i) {
        base_data[i] = rand() % 256;
    }

    for (int i = 0; i < MAX_VARIANTS; ++i) {
        memcpy(variants[i], base_data, INPUT_LEN);
        if (inject_fault && rand() % 2 == 0) {
            injected = true;
            injected_faults++;
            data_diversity_add_noise(variants[i], INPUT_LEN, NOISE_LEVEL + 3);
        }
        else {
            data_diversity_add_noise(variants[i], INPUT_LEN, NOISE_LEVEL);
        }
        inputs[i] = variants[i];
        outputs[i] = &results[i];
    }

    success = data_diversity_execute(
        process_signal,
        compare_uint32,
        inputs,
        MAX_VARIANTS,
        outputs,
        VOTING_MAJORITY,
        "noise_variation_case"
    );

    if (injected && !success) true_positives++;
    else if (injected && success) false_negatives++;
    else if (!injected && success) true_negatives++;
    if (!success) detected_errors++;
}

void run_format_variation_case(bool inject_fault) {
    const void* inputs[MAX_VARIANTS];
    void* outputs[MAX_VARIANTS];
    uint8_t variants[MAX_VARIANTS][INPUT_LEN];
    uint32_t results[MAX_VARIANTS];
    bool injected = false;
    bool success = true;

    for (int i = 0; i < MAX_VARIANTS; ++i) {
        for (int j = 0; j < INPUT_LEN; ++j) {
            variants[i][j] = (i % 2 == 0) ? j : 255 - j;
        }

        if (inject_fault && i == MAX_VARIANTS - 1) {
            injected = true;
            injected_faults++;
            variants[i][0] ^= 0xFF;
        }

        inputs[i] = variants[i];
        outputs[i] = &results[i];
    }

    success = data_diversity_execute(
        process_signal,
        compare_uint32,
        inputs,
        MAX_VARIANTS,
        outputs,
        VOTING_STRICT_COMPARE,
        "format_variation_case"
    );

    if (injected && !success) true_positives++;
    else if (injected && success) false_negatives++;
    else if (!injected && success) true_negatives++;
    if (!success) detected_errors++;
}

void data_diversity_snippet() {
    srand(12345);

    for (int i = 0; i < ITERATIONS; ++i) {
        total_runs++;
        bool inject = ((float)rand() / RAND_MAX) < 0.2;

        if (rand() % 2 == 0) {
            run_noise_variation_case(inject);
        }
        else {
            run_format_variation_case(inject);
        }
    }

    platform_print("\n=== DATA DIVERSITY EXPERIMENT REPORT ===\n");

    /*printf("Total iterations     : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Detected mismatches  : %d\n", detected_errors);
    printf("True Positives (TP)  : %d\n", true_positives);
    printf("False Negatives (FN) : %d\n", false_negatives);
    printf("True Negatives (TN)  : %d\n", true_negatives);

    float recall = (true_positives + false_negatives) ? (float)true_positives / (true_positives + false_negatives) : 0;
    float f1 = (recall > 0) ? 2 * (recall * 100) / (recall * 100 + 100) : 0;

    printf("Recall   : %.2f%%\n", recall * 100);
    printf("F1-score : %.2f\n", f1); */
}
