#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "reliability_mechanisms/single_design/error_detection/deadline_monitor.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"


#define ITERATIONS 1000
#define DEADLINE_US 10000       // 10ms
#define SENSOR_DEADLINE_US 15000
#define ERROR_PROBABILITY 0.2
#define FN_PROBABILITY 0.1

static int total_runs = 0;
static int injected_faults = 0;
static int detected_errors = 0;
static int true_positives = 0;
static int false_negatives = 0;
static int true_negatives = 0;

deadline_monitor_t monitor;

// Кейс 1: вычислительная задача с ограничением по времени
void run_timed_task_case(bool inject_fault) {
    deadline_monitor_init(&monitor, DEADLINE_US);

    deadline_monitor_start(&monitor);

    bool injected = false, detected = false;

    if (inject_fault) {
        injected = true;
        injected_faults++;

        if (((float)rand() / RAND_MAX) < FN_PROBABILITY) {
            //sleep_us(DEADLINE_US - 2000); // почти укладывается
        }
        else {
            //sleep_us(DEADLINE_US + 5000); // нарушает
        }
    }
    else {
        //sleep_us(DEADLINE_US - 3000); // всегда укладывается
    }

    deadline_monitor_stop(&monitor, __func__);

    if (monitor.timing_violation) {
        detected = true;
        detected_errors++;
    }

    if (injected && detected) true_positives++;
    else if (injected && !detected) false_negatives++;
    else if (!injected && !detected) true_negatives++;
}

int deadline_monitor_snippet() {
    srand(12345);

    for (int i = 0; i < ITERATIONS; i++) {
        total_runs++;
        bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;

        run_timed_task_case(inject);
    }

    printf("\n=== DEADLINE MONITOR EXPERIMENT REPORT ===\n");
    /* printf("Total iterations     : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Detected violations  : %d\n", detected_errors);
    printf("True Positives (TP)  : %d\n", true_positives);
    printf("False Negatives (FN) : %d\n", false_negatives);
    printf("True Negatives (TN)  : %d\n", true_negatives);

    float recall = (true_positives + false_negatives) ? (float)true_positives / (true_positives + false_negatives) : 0;
    float f1 = (recall > 0) ? 2 * (recall * 100) / (recall * 100 + 100) : 0;

    printf("Recall   : %.2f%%\n", recall * 100);
    printf("F1-score : %.2f\n", f1); */

    return 0;
}
