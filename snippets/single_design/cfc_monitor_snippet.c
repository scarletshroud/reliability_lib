#include "cfc_monitor_snippet.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "reliability_mechanisms/single_design/error_detection/cfc_monitor.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 1000
#define ERROR_PROBABILITY 0.2

#define SIG_IDLE    0x10
#define SIG_INIT    0x20
#define SIG_ACTIVE  0x30
#define SIG_DONE    0x40
#define SIG_EXIT_OK 0x50
#define SIG_FAULT   0xDEAD

static float FN_PROBABILITY = 0.01;

static int total_runs = 0;
static int injected_faults = 0;
static int detected_errors = 0;
static int false_negatives = 0;
static int true_positives = 0;
static int true_negatives = 0;
static int total_time = 0;

void run_fsm_case(cfc_monitor_t* monitor, bool inject_fault) {
    bool detected = false;

    CFC_ENTER(monitor, SIG_IDLE);
    if (!CFC_CHECK(monitor, SIG_IDLE)) detected = true;

    if (inject_fault) {
        uint32_t wrong_sig = ((float)rand() / RAND_MAX < FN_PROBABILITY) ? SIG_INIT : SIG_INIT + 1;

        CFC_ENTER(monitor, wrong_sig);

        if (wrong_sig != SIG_INIT) injected_faults++;
    }
    else {
        CFC_ENTER(monitor, SIG_INIT);
    }
    if (!CFC_CHECK(monitor, SIG_INIT)) detected = true;

    CFC_ENTER(monitor, SIG_ACTIVE);
    if (!CFC_CHECK(monitor, SIG_ACTIVE)) detected = true;

    CFC_ENTER(monitor, SIG_DONE);
    if (!CFC_CHECK(monitor, SIG_DONE)) detected = true;

    if (inject_fault && detected) true_positives++;
    else if (inject_fault && !detected) false_negatives++;
    else if (!inject_fault && !detected) true_negatives++;
}

void return_with_monitor(cfc_monitor_t* monitor, bool inject_fault) {
    if (inject_fault) {
        uint32_t sig = ((float)rand() / RAND_MAX < FN_PROBABILITY) ? SIG_EXIT_OK : SIG_EXIT_OK + 1;
        if (sig != SIG_EXIT_OK) {
            injected_faults++;
        }
        CFC_ENTER(monitor, sig);
    }
    else {
        CFC_ENTER(monitor, SIG_EXIT_OK);
    }
}

void run_return_case(cfc_monitor_t* monitor, bool inject_fault) {
    bool injected = false, detected = false;

    CFC_ENTER(monitor, SIG_IDLE);
    if (!CFC_CHECK(monitor, SIG_IDLE)) detected = true;

    return_with_monitor(monitor, inject_fault);
    if (!CFC_CHECK(monitor, SIG_EXIT_OK)) detected = true;

    if (inject_fault && detected) true_positives++;
    else if (inject_fault && !detected) false_negatives++;
    else if (!inject_fault && !detected) true_negatives++;
}

#define CFC_CHECK_TIME

void cfc_monitor_snippet() {
    srand(12345);

    total_runs = 0;
    injected_faults = 0;
    detected_errors = 0;
    false_negatives = 0;
    true_positives = 0;
    true_negatives = 0;
    error_monitor_clear();

    for (int i = 0; i < ITERATIONS; i++) {
        total_runs++;
        cfc_monitor_t monitor;
        cfc_monitor_init(&monitor);

#ifdef CFC_CHECK_TIME
        for (int j = 0; j < ITERATIONS; ++j) {
            uint32_t f = (uint32_t)(rand() % 5 * 10);
            uint32_t s = (uint32_t)(rand() % 5 * 10);
            int start = get_time_us();

            CFC_ENTER(&monitor, f);
            CFC_CHECK(&monitor, s);

            total_time += get_time_us() - start;
        }
#endif

        bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;

        if (rand() % 2 == 0) {
            run_fsm_case(&monitor, inject);
        }
        else {
            run_return_case(&monitor, inject);
        }
    }

    platform_print("\n=== FINAL CFC MONITOR EXPERIMENT REPORT ===\n");

    /*printf("FN_PROBABILITY  : %f\n", FN_PROBABILITY);
    printf("Total iterations     : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Detected CFC errors  : %d\n", error_monitor_err_cnt());
    printf("False Negatives (FN) : %d\n", false_negatives);
    printf("True Negatives (TN)  : %d\n", true_negatives);

    */
}

int cfc_main() {
    float fn_probs[] = { 0.0, 0.05, 0.10, 0.15, 0.20, 0.30 };
    int count = sizeof(fn_probs) / sizeof(fn_probs[0]);

    for (int i = 0; i < count; i++) {
        FN_PROBABILITY = fn_probs[i];
        cfc_snippet();
    }

    return 0;
}
