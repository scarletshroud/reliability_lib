#include "input_validator_snippet.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "reliability_mechanisms/single_design/error_detection/input_validation.h"
#include "error_monitor/error_monitor.h"
#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 1000
#define ERROR_PROBABILITY 0.2
#define FN_PROBABILITY 0.01

typedef struct {
    int min;
    int max;
} range_context_t;

typedef struct {
    size_t max_length;
} strlen_context_t;

static bool validate_range(const void* value, const void* context) {
    const int* val = (const int*)value;
    const range_context_t* range = (const range_context_t*)context;
    return (*val >= range->min) && (*val <= range->max);
}

static bool validate_string_length(const void* value, const void* context) {
    const char* str = (const char*)value;
    const strlen_context_t* len = (const strlen_context_t*)context;
    return strlen(str) <= len->max_length;
}

static int total_runs = 0;
static int injected_faults = 0;
static int detected_errors = 0;
static int true_positives = 0;
static int false_negatives = 0;
static int true_negatives = 0;

static void run_range_case(bool inject) {
    int param;
    bool injected = false, detected = false;

    range_context_t range = { 0, 100 };
    validator_t val = { "param", validate_range, &range };

    if (inject) {
        injected = true;
        injected_faults++;
        param = 200;  // заведомо вне диапазона

        if (((float)rand() / RAND_MAX) < FN_PROBABILITY) {
            param = 100;  // допустимый, FN
        }
    }
    else {
        param = rand() % 101;
    }

    if (!input_validate_field(&val, &param)) {
        detected_errors++;
        detected = true;
    }

    if (injected && detected) true_positives++;
    else if (injected && !detected) false_negatives++;
    else if (!injected && !detected) true_negatives++;
}

static void run_format_case(bool inject) {
    char buffer[64];
    bool injected = false, detected = false;

    strlen_context_t ctx = { 20 };
    validator_t val = { "message", validate_string_length, &ctx };

    if (inject) {
        injected = true;
        injected_faults++;
        if (((float)rand() / RAND_MAX) < FN_PROBABILITY) {
            snprintf(buffer, sizeof(buffer), "1234567890123456789");  // ok
        }
        else {
            snprintf(buffer, sizeof(buffer), "this string is way too long for the validator");
        }
    }
    else {
        snprintf(buffer, sizeof(buffer), "valid");
    }

    if (!input_validate_field(&val, buffer)) {
        detected_errors++;
        detected = true;
    }

    if (injected && detected) true_positives++;
    else if (injected && !detected) false_negatives++;
    else if (!injected && !detected) true_negatives++;
}

void input_validator_snippet(void) {
    srand(12345);

    for (int i = 0; i < ITERATIONS; i++) {
        total_runs++;
        bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;

        if (rand() % 2 == 0) {
            run_range_case(inject);
        }
        else {
            run_format_case(inject);
        }
    }

    platform_print("\n=== INPUT VALIDATION EXPERIMENT REPORT ===\n");

    /*printf("Total iterations     : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Detected violations  : %d\n", detected_errors);
    printf("True Positives (TP)  : %d\n", true_positives);
    printf("False Negatives (FN) : %d\n", false_negatives);
    printf("True Negatives (TN)  : %d\n", true_negatives);

    float recall = (true_positives + false_negatives) ? (float)true_positives / (true_positives + false_negatives) : 0;
    float f1 = (recall > 0) ? 2 * (recall * 100) / (recall * 100 + 100) : 0;

    printf("Recall   : %.2f%%\n", recall * 100);
    printf("F1-score : %.2f\n", f1); */
}
