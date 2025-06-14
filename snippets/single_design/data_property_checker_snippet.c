#include "data_property_checker_snippet.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "reliability_mechanisms/single_design/error_detection/data_property_checker.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 1000
#define ERROR_PROBABILITY 0.2
#define FN_PROBABILITY 0.01

// Структура 1: связный список
typedef struct Node {
    int value;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    int count;
} LinkedList;

// Инвариант: список должен содержать count элементов
bool check_linked_list_consistency(const void* structure) {
    const LinkedList* list = (const LinkedList*)structure;
    Node* current = list->head;
    int actual_count = 0;
    while (current) {
        actual_count++;
        current = current->next;
    }
    return actual_count == list->count;
}

// Структура 2: FSM
typedef enum {
    STATE_IDLE,
    STATE_PROCESS,
    STATE_DONE,
    STATE_INVALID
} fsm_state_t;

typedef struct {
    fsm_state_t state;
    bool ready;
} FSM;

bool check_fsm_valid_state(const void* structure) {
    const FSM* fsm = (const FSM*)structure;
    return fsm->state >= STATE_IDLE && fsm->state <= STATE_DONE;
}

// Статистика
static int total_runs = 0;
static int injected_faults = 0;
static int detected_errors = 0;
static int true_positives = 0;
static int false_negatives = 0;
static int true_negatives = 0;

void run_list_invariant_case(bool inject) {
    LinkedList list = { 0 };
    bool injected = false, detected = false;

    Node n1 = { 1, NULL }, n2 = { 2, NULL };
    list.head = &n1;
    n1.next = &n2;
    list.count = 2;

    if (inject) {
        injected = true;
        injected_faults++;
        if ((float)rand() / RAND_MAX < FN_PROBABILITY) {
            list.count = 2;  // корректно
        }
        else {
            list.count = 1;  // несоответствие
        }
    }

    property_checker_t checker = { "linked list consistency", check_linked_list_consistency };

    if (!data_property_check_single(&checker, &list)) {
        detected_errors++;
        detected = true;
    }

    if (injected && detected) true_positives++;
    else if (injected && !detected) false_negatives++;
    else if (!injected && !detected) true_negatives++;
}

void run_fsm_state_case(bool inject) {
    FSM fsm = { STATE_PROCESS, true };
    bool injected = false, detected = false;

    if (inject) {
        injected = true;
        injected_faults++;
        if ((float)rand() / RAND_MAX < FN_PROBABILITY) {
            fsm.state = STATE_DONE;  // допустимый
        }
        else {
            fsm.state = STATE_INVALID;
        }
    }

    property_checker_t checker = { "fsm state validity", check_fsm_valid_state };

    if (!data_property_check_single(&checker, &fsm)) {
        detected_errors++;
        detected = true;
    }

    if (injected && detected) true_positives++;
    else if (injected && !detected) false_negatives++;
    else if (!injected && !detected) true_negatives++;
}

void data_property_checker_snippet() {
    srand(12345);

    for (int i = 0; i < ITERATIONS; ++i) {
        total_runs++;
        bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;

        if (rand() % 2 == 0) {
            run_list_invariant_case(inject);
        }
        else {
            run_fsm_state_case(inject);
        }
    }

    platform_print("\n=== DATA PROPERTY CHECKER EXPERIMENT REPORT ===\n");

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
