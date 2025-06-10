#include "checkpoint_restart_snippet.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "reliability_mechanisms/single_design/checkpoint_restart.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 1000
#define ERROR_PROBABILITY 0.2
#define FN_PROBABILITY 0.01

typedef struct {
    int pos;
    int vel;
    int error_counter;
} SystemState;

static int total_runs = 0;
static int injected_faults = 0;
static int successful_recovery = 0;
static int failed_recovery = 0;
static int masked_errors = 0;

static void corrupt_state(SystemState* state, bool inject_fault) {
    if (inject_fault) {
        injected_faults++;
        if ((float)rand() / RAND_MAX > FN_PROBABILITY) {
            state->error_counter = 999;
        }
    }
}

static void run_static_checkpoint_case() {
    SystemState current = { 0, 1, 0 };
    SystemState static_ref;
    memcpy(&static_ref, &current, sizeof(SystemState));

    checkpoint_t static_cp = {
        .state_buffer = &static_ref,
        .size = sizeof(SystemState)
    };

    bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;
    corrupt_state(&current, inject);

    if (inject) {
        if (current.error_counter == 999) {
            bool restored = checkpoint_static_restart(&static_cp, &current);
            if (restored) successful_recovery++;
            else failed_recovery++;
        }
        else {
            masked_errors++;
        }
    }
}

static void run_dynamic_checkpoint_case() {
    SystemState current = { 0, 2, 0 };
    SystemState dynamic_buf = { 0 };

    checkpoint_t dyn_cp = {
        .state_buffer = &dynamic_buf,
        .size = sizeof(SystemState)
    };

    checkpoint_dynamic_save(&dyn_cp, &current);

    bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;
    corrupt_state(&current, inject);

    if (inject) {
        if (current.error_counter == 999) {
            bool restored = checkpoint_dynamic_restore(&dyn_cp, &current);
            if (restored) successful_recovery++;
            else failed_recovery++;
        }
        else {
            masked_errors++;
        }
    }
}

void checkpoint_restart_snippet(void) {
    srand((unsigned int)time(NULL));
    platform_print("\n=== STATIC CHECKPOINT TEST ===\n");

    for (int i = 0; i < ITERATIONS; i++) {
        total_runs++;
        run_static_checkpoint_case();
    }

     /*
    printf("Total runs           : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Recovered (success)  : %d\n", successful_recovery);
    printf("Recovery failed      : %d\n", failed_recovery);
    printf("Masked errors        : %d\n", masked_errors);
    */

    total_runs = injected_faults = successful_recovery = failed_recovery = masked_errors = 0;

    platform_print("\n=== DYNAMIC CHECKPOINT TEST ===\n");

    for (int i = 0; i < ITERATIONS; i++) {
        total_runs++;
        run_dynamic_checkpoint_case();
    }
/*
    printf("Total runs           : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Recovered (success)  : %d\n", successful_recovery);
    printf("Recovery failed      : %d\n", failed_recovery);
    printf("Masked errors        : %d\n", masked_errors);
    */
}
