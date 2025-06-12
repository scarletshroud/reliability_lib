#include "n_version_snippet.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "reliability_mechanisms/design_diversity/n_version.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 1000
#define FN_PROBABILITY 0.01
#define FP_PROBABILITY 0.01

static int tp = 0, fn = 0, fp = 0, tn = 0, injected = 0;

static bool version_ok(void* result, const void* input) {
    const int* in = (const int*)input;
    int* out = (int*)result;
    *out = *in + 1;
    return true;
}

static bool version_buggy_minor(void* result, const void* input) {
    const int* in = (const int*)input;
    int* out = (int*)result;
    *out = *in + 2;  // почти корректный результат
    return true;
}

static bool version_buggy_major(void* result, const void* input) {
    const int* in = (const int*)input;
    int* out = (int*)result;
    *out = *in + 100;  // явно неверный результат
    return true;
}

static bool version_fp_noise(void* result, const void* input) {
    const int* in = (const int*)input;
    int* out = (int*)result;
    *out = *in + ((rand() % 3) - 1);  // ±1
    return true;
}

static bool compare_strict(const void* a, const void* b) {
    return *(const int*)a == *(const int*)b;
}

static void copy_int(void* dst, const void* src) {
    *(int*)dst = *(const int*)src;
}

void run_majority_case_with_real_fn() {
    int input = rand() % 100;
    int res1, res2, res3, selected = 0;
    void* buffers[3] = { &res1, &res2, &res3 };

    bool inject = ((float)rand() / RAND_MAX) < 0.2;
    bool real_fn = false;

    n_version_fn_t v1 = version_ok;
    n_version_fn_t v2 = version_ok;
    n_version_fn_t v3 = version_ok;

    if (inject) {
        injected++;
        float r = (float)rand() / RAND_MAX;
        if (r < 0.33) {
            v1 = version_buggy_minor;
            v2 = version_buggy_minor;
            v3 = version_ok;
            real_fn = true; // голосование выберет неправильный результат
        }
        else if (r < 0.66) {
            v1 = version_ok;
            v2 = version_buggy_major;
            v3 = version_buggy_major;
            real_fn = true; // голосование сломано
        }
        else {
            v1 = version_ok;
            v2 = version_ok;
            v3 = version_buggy_major;
            // обычный случай TP
        }
    }

    n_version_exec_t exec = {
        .versions = {v1, v2, v3},
        .version_count = 3,
        .compare = compare_strict,
        .copy = copy_int,
        .mode = N_VERSION_VOTING_MAJORITY,
        .context = "majority_case_fn"
    };

    bool ok = n_version_execute(&exec, &input, buffers, &selected);

    if (inject && real_fn && ok) {
        fn++; // ошибка проявилась, но не была замечена
    }
    else if (inject && !real_fn && ok) {
        tp++; // обычный TP
    }
    else if (!inject && ok) {
        tn++;
    }
    else if (!inject && !ok) {
        fp++;
    }
}

void n_version_snippet() {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < ITERATIONS; ++i) {
        run_majority_case_with_real_fn();
    }

    printf("\n=== N-VERSION EXPERIMENT: WITH REAL FN ===\n");
//    printf("Injected faults     : %d\n", injected);
//    printf("True Positives (TP) : %d\n", tp);
//    printf("False Negatives (FN): %d\n", fn);
//    printf("False Positives (FP): %d\n", fp);
//    printf("True Negatives (TN) : %d\n", tn);
//
//    float recall = (tp + fn) ? (float)tp / (tp + fn) : 0;
//    float precision = (tp + fp) ? (float)tp / (tp + fp) : 0;
//    float f1 = (precision + recall) ? 2 * (precision * recall) / (precision + recall) : 0;
//
//    printf("Recall   : %.2f%%\n", recall * 100);
//    printf("Precision: %.2f%%\n", precision * 100);
//    printf("F1-score : %.2f\n", f1);
}
