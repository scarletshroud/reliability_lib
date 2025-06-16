#include "recovery_block_snippet.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ITERATIONS 1000

#include "reliability_mechanisms/design_diversity/recovery_block.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

static int tp = 0, fn = 0, tn = 0, fp = 0, injected = 0;

bool primary_faulty(void* result, const void* input) {
    *(int*)result = (*(int*)input + 1) * 2;  // неправильная логика
    return true;
}

bool alternative_correct(void* result, const void* input) {
    *(int*)result = *(int*)input + 1;
    return true;
}

bool acceptance_test(const void* result) {
    const int* val = (const int*)result;
    return (*val % 2 == 1);  // Ожидаем нечетное число (input + 1)
}

static void copy_int(void* dest, const void* src) {
    *(int*)dest = *(const int*)src;
}

void run_recovery_block_case_1() {
    int input = rand() % 100;
    int expected = input + 1;
    int result = 0;

    injected++;

    recovery_block_t rb = {
        .variants = {primary_faulty, alternative_correct},
        .variant_count = 2,
        .acceptance_test = acceptance_test,
        .mode = RB_MODE_SEQUENTIAL,
        .context = "RB_Case_1"
    };

    bool ok = recovery_block_execute(&rb, &input, &result);

    if (ok && result == expected) {
        tp++;
    }
    else if (ok && result != expected) {
        fn++;
    }
    else if (!ok) {
        fn++;  // если вообще ни одна реализация не прошла
    }
}

void recovery_block_snippet() {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < ITERATIONS; ++i) {
        run_recovery_block_case_1();
    }

    platform_print("\n=== RECOVERY BLOCK EXPERIMENT: CASE 1 ===\n");
    /*printf("Injected faults       : %d\n", injected);
    printf("True Positives (TP)   : %d\n", tp);
    printf("False Negatives (FN)  : %d\n", fn);

    float recall = (tp + fn) ? (float)tp / (tp + fn) : 0;
    float precision = tp ? (float)tp / (tp + fn) : 0;
    float f1 = (precision + recall) ? 2 * (precision * recall) / (precision + recall) : 0;

    printf("Recall    : %.2f%%\n", recall * 100);
    printf("F1-score  : %.2f\n", f1); */
}
