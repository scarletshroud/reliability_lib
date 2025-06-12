#include "n_version_t_snippet.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "reliability_mechanisms/design_diversity/n_version.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 500

int tp = 0, fn = 0, fp = 0, tn = 0, injected = 0;

bool version_ok(void* result, const void* input) {
    *(int*)result = *(const int*)input + 1;
    return true;
}

bool version_fault_a(void* result, const void* input) {
    *(int*)result = *(const int*)input + 100;
    return true;
}

bool version_fault_b(void* result, const void* input) {
    *(int*)result = *(const int*)input + 101;
    return true;
}

bool version_fault_c(void* result, const void* input) {
    *(int*)result = *(const int*)input + 102;
    return true;
}

bool version_fault_minor(void* result, const void* input) {
    *(int*)result = *(const int*)input + 2;
    return true;
}

bool compare_strict(const void* a, const void* b) {
    return *(const int*)a == *(const int*)b;
}

void copy_int(void* dst, const void* src) {
    *(int*)dst = *(const int*)src;
}

void run_threshold_case_major_fault_divergent() {
    int input = rand() % 100;
    int expected = input + 1;
    int res[5] = { 0 };
    void* buffers[5] = { &res[0], &res[1], &res[2], &res[3], &res[4] };
    injected++;

    n_version_exec_t exec = {
        .versions = {version_ok, version_ok, version_fault_a, version_fault_b, version_fault_c},
        .version_count = 5,
        .threshold = 3,
        .compare = compare_strict,
        .copy = copy_int,
        .mode = N_VERSION_VOTING_THRESHOLD,
        .context = "threshold_major_divergent"
    };

    int selected = 0;
    bool ok = n_version_execute(&exec, &input, buffers, &selected);

    if (!ok)
        tp++;
    else if (selected != expected)
        fn++;
    else
        fn++;
}

void run_threshold_case_minor_fault() {
    int input = rand() % 100;
    int expected = input + 1;
    int res[5] = { 0 };
    void* buffers[5] = { &res[0], &res[1], &res[2], &res[3], &res[4] };

    n_version_exec_t exec = {
        .versions = {version_ok, version_ok, version_ok, version_ok, version_fault_minor},
        .version_count = 5,
        .threshold = 3,
        .compare = compare_strict,
        .copy = copy_int,
        .mode = N_VERSION_VOTING_THRESHOLD,
        .context = "threshold_minor_fault"
    };

    int selected = 0;
    bool ok = n_version_execute(&exec, &input, buffers, &selected);

    if (ok && selected == expected)
        tn++;
    else
        fp++;
}

void n_version_t_snippet(void) {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < ITERATIONS; ++i) {
        run_threshold_case_major_fault_divergent();
        run_threshold_case_minor_fault();
    }

    printf("\\n=== N-VERSION EXPERIMENT: THRESHOLD MODE (DIVERGENT) ===\\n");
    printf("Injected faults (major)  : %d\n", injected);
    printf("True Positives (TP)      : %d\n", tp);
    printf("False Negatives (FN)     : %d\n", fn);
    printf("False Positives (FP)     : %d\n", fp);
    printf("True Negatives (TN)      : %d\n", tn);

    float recall = (tp + fn) ? (float)tp / (tp + fn) : 0;
    float precision = (tp + fp) ? (float)tp / (tp + fp) : 0;
    float f1 = (precision + recall) ? 2 * (precision * recall) / (precision + recall) : 0;

    printf("Recall   : %.2f%%\\n", recall * 100);
    printf("Precision: %.2f%%\\n", precision * 100);
    printf("F1-score : %.2f\\n", f1);
}
