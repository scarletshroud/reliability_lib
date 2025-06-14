#include "exception_handler_snippet.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "reliability_mechanisms/single_design/exception_handler.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 1000
#define ERROR_PROBABILITY 0.2
#define FN_PROBABILITY 0.01

int total_runs = 0;
int injected_faults = 0;
int detected_exceptions = 0;
int true_positives = 0;
int false_negatives = 0;
int true_negatives = 0;

static int local_exception_counter = 0;

void my_exception_logger(ExceptionType ex_type, const char* file, const char* func, uint32_t line) {
    local_exception_counter++;
}

void run_div_by_zero_case(bool inject_fault) {
    bool injected = false, detected = false;
    int a = 100, b = 10, c = 0;

    if (inject_fault) {
        injected = true;
        injected_faults++;
        if ((float)rand() / RAND_MAX > FN_PROBABILITY) {
            b = 0;  // приведёт к исключению
        }
    }

    if (b == 0) {
        EXCEPTION_RAISE(EXCEPTION_DIVISION_BY_ZERO);
        detected = true;
    }
    else {
        c = a / b;
        (void)c;
    }

    if (inject_fault && detected) true_positives++;
    else if (inject_fault && !detected) false_negatives++;
    else if (!inject_fault && !detected) true_negatives++;
}

void run_invalid_pointer_case(bool inject_fault) {
    bool injected = false, detected = false;
    char* ptr = malloc(10);

    if (inject_fault) {
        injected = true;
        injected_faults++;
        if ((float)rand() / RAND_MAX > FN_PROBABILITY) {
            free(ptr);
            ptr = NULL;
        }
    }

    if (ptr == NULL) {
        EXCEPTION_RAISE(EXCEPTION_MEMORY_CORRUPTION);
        detected = true;
    }
    else {
        ptr[0] = 'A';
        free(ptr);
    }

    if (inject_fault && detected) true_positives++;
    else if (inject_fault && !detected) false_negatives++;
    else if (!inject_fault && !detected) true_negatives++;
}

void exception_handler_snippet(void) {
    srand(12345);
    
    exception_handler_init(my_exception_logger);

    for (int i = 0; i < ITERATIONS; i++) {
        total_runs++;
        bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;

        if (rand() % 2 == 0) {
            run_div_by_zero_case(inject);
        }
        else {
            run_invalid_pointer_case(inject);
        }
    }

    platform_print("\n=== EXCEPTION HANDLER EXPERIMENT REPORT ===\n");
    /*printf("Total iterations     : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Detected exceptions  : %d\n", local_exception_counter);
    printf("True Positives (TP)  : %d\n", true_positives);
    printf("False Negatives (FN) : %d\n", false_negatives);
    printf("True Negatives (TN)  : %d\n", true_negatives);

    float recall = (true_positives + false_negatives) ? (float)true_positives / (true_positives + false_negatives) : 0;
    float f1 = (recall > 0) ? 2 * (recall * 100) / (recall * 100 + 100) : 0;

    printf("Recall   : %.2f%%\n", recall * 100);
    printf("F1-score : %.2f\n", f1);
    */
}
