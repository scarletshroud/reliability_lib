#include <assertion_snippet.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ENABLE_ASSERTS 1

#include "reliability_mechanisms/single_design/error_detection/improved_assertion.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 1000
#define ERROR_PROBABILITY 0.2

static uint32_t total_runs = 0;
static uint32_t injected_faults = 0;
static uint32_t detected_errors = 0;

/* -- Range Violation Case -- */
static void test_range_violation() {
    int temp;
    uint8_t fault = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;

    if (fault) {
        temp = (rand() % 200) + 200;  // вне диапазона [200,399]
        injected_faults++;
    }
    else {
        temp = (rand() % 166) - 40;   // в диапазоне [-40,125]
    }

    ASSERT(temp >= -40 && temp <= 125, ASSERT_LEVEL_ERROR);
}

/* -- Unitilized State Case -- */
static void test_state_flag() {
    bool state;

    uint8_t fault = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;
    if (fault) {
        state = false;
        injected_faults++;
    }
    else {
        state = true;
    }
    ASSERT(state == true, ASSERT_LEVEL_ERROR);
}

/* -- Null Pointer Case -- */
static void test_null_pointer() {
    uint8_t fault = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;

    char* ptr = fault ? NULL : malloc(1);
    if (fault) injected_faults++;

    ASSERT(ptr != NULL, ASSERT_LEVEL_CRITICAL);

    if (ptr) {
        ptr[0] = 'A';
        free(ptr);
    }
}

void assertion_snippet(void) {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < ITERATIONS; i++) {
        total_runs++;

        int choice = rand() % 3;
        switch (choice) {
            case 0:
                test_range_violation();
                break;
            case 1:
                test_state_flag();
                break;
            case 2:
                test_null_pointer();
                break;
        }
    }

    detected_errors = error_monitor_err_cnt();

    platform_print("\n=== ASSERTION EXPERIMENT REPORT ===\n");
/*  printf("Total iterations     : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Detected ASSERTs     : %d\n", detected_errors);
    printf("Detection rate       : %.2f%%\n", injected_faults ? 100.0 * detected_errors / injected_faults : 0.0);
    printf("MTTF (approx)        : %.2f iterations\n", detected_errors ? (float)total_runs / detected_errors : (float)total_runs); */
}
