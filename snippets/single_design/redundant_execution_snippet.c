#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "reliability_mechanisms/single_design/error_detection/redundant_execution.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 1000
#define ERROR_PROBABILITY 0.2
#define FN_PROBABILITY 0.01

// Типы данных
typedef struct {
    int a;
    int b;
} input_data_t;

typedef struct {
    int result;
    bool path_A_taken;
} output_data_t;

// Глобальная статистика
static int total_runs = 0;
static int injected_faults = 0;
static int detected_errors = 0;
static int true_positives = 0;
static int false_negatives = 0;
static int true_negatives = 0;
static bool injected = false;
static bool first_one = true;
static bool second_one = false;

// С испорченным вариантом
static void compute_faulty(output_data_t* out, const input_data_t* in) {
    int tmp_result = in->a + in->b;

    if (first_one) {
        if (((float)rand() / RAND_MAX) < ERROR_PROBABILITY) {
            injected = true;

            injected_faults++;

            if (((float)rand() / RAND_MAX) < FN_PROBABILITY) {
                // мягкое искажение: не влияет на результат
                tmp_result += 0;
            }
            else {
                // грубое искажение результата
                tmp_result += (rand() % 10) + 1;
            }
        }
    }

    out->result = tmp_result;

    if (tmp_result % 2 == 0) {
        out->path_A_taken = true;
    }
    else {
        out->path_A_taken = false;
    }

    if (first_one) {
        first_one = false;
        second_one = true;
    }
    else {
        first_one = true;
        second_one = false;
    }
}

// Сравнение результатов
static bool compare_outputs(const output_data_t* a, const output_data_t* b) {
    return a->result == b->result && a->path_A_taken == b->path_A_taken;
}

void redundant_execution_snippet(void) {
    srand(12345);

    for (int i = 0; i < ITERATIONS; ++i) {
        total_runs++;
        input_data_t in = { rand() % 100, rand() % 100 };
        output_data_t ref_out = { 0 }, test_out = { 0 };
        injected = false;

        bool mismatch = !redundant_execute_noargs(compute_faulty, compare_outputs, &in, &ref_out, &test_out);

        if (injected && mismatch) {
            detected_errors++;
            true_positives++;
        }
        else if (injected && !mismatch) {
            false_negatives++;
        }
        else if (!injected && !mismatch) {
            true_negatives++;
        }
    }

    platform_print("\n=== REDUNDANT EXECUTION EXPERIMENT REPORT ===\n");
    /*printf("Total iterations     : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Detected errors      : %d\n", error_monitor_err_cnt());
    printf("True Positives (TP)  : %d\n", true_positives);
    printf("False Negatives (FN) : %d\n", false_negatives);
    printf("True Negatives (TN)  : %d\n", true_negatives);

    float recall = (true_positives + false_negatives) ? (float)true_positives / (true_positives + false_negatives) : 0;
    float f1 = (recall > 0) ? 2 * (recall * 100) / (recall * 100 + 100) : 0;

    printf("Recall   : %.2f%%\n", recall * 100);
    printf("F1-score : %.2f\n", f1); */
}
