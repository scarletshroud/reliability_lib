
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "reliability_mechanisms/single_design/process_pair.h"
#include "error_monitor/error_monitor.h"
#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 500
#define ERROR_PROBABILITY 0.2
#define FN_PROBABILITY 0.05
#define FP_PROBABILITY 0.05

static int tp = 0, fn = 0, tn = 0, fp = 0, injected = 0;

bool active_process_1(void* result, const void* input) {
    int in = *(const int*)input;
    *(int*)result = in + 10;

    if ((float)rand() / RAND_MAX < ERROR_PROBABILITY) {
        if ((float)rand() / RAND_MAX > FN_PROBABILITY) {
            *(int*)result = in + 99;  // нарушаем результат
        }
        return true;
    }
    return true;
}

bool active_process_2(void* result, const void* input) {
    int in = *(const int*)input;
    *(int*)result = in + 10;

    if ((float)rand() / RAND_MAX < FP_PROBABILITY) {
        *(int*)result = in + 20;  // создаём расхождение
    }
    return true;
}

static bool compare_ints(const void* a, const void* b) {
    return *(const int*)a == *(const int*)b;
}

void run_process_pair_active_active_case(bool inject_fault) {
    int input = rand() % 100;
    int result_main = 0, result_backup = 0;

    process_pair_t pair = {
        .active = active_process_1,
        .backup = active_process_2,
        .compare = compare_ints,
        .mode = PROCESS_PAIR_ACTIVE_ACTIVE,
        .context = "active_active_case"
    };

    bool ok = process_pair_execute(&pair, &input, &result_main, &result_backup);

    if (inject_fault) {
        injected++;
        if (ok) tp++;
        else fn++;
    }
    else {
        if (ok) tn++;
        else fp++;
    }
}

int process_pair_active_snippet() {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < ITERATIONS; ++i) {
        bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;
        run_process_pair_active_active_case(inject);
    }

    platform_print("\n=== PROCESS PAIR (ACTIVE-ACTIVE) REPORT ===\n");

    /*printf("Injected faults     : %d\n", injected);
    printf("True Positives (TP) : %d\n", tp);
    printf("False Negatives (FN): %d\n", fn);
    printf("True Negatives (TN) : %d\n", tn);
    printf("False Positives (FP): %d\n", fp);

    float recall = (tp + fn) ? (float)tp / (tp + fn) : 0;
    float precision = (tp + fp) ? (float)tp / (tp + fp) : 0;
    float f1 = (precision + recall) ? 2 * (precision * recall) / (precision + recall) : 0;

    printf("Recall   : %.2f%%\n", recall * 100);
    printf("Precision: %.2f%%\n", precision * 100);
    printf("F1-score : %.2f\n", f1); */

    return 0;
}
