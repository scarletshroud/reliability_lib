#include "checksum_snippet.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "reliability_mechanisms/single_design/error_detection/checksum_validator.h"

#define ITERATIONS 1000
#define ERROR_PROBABILITY 0.2
#define FN_PROBABILITY 0.01
#define DATA_LENGTH 16

static uint32_t total_runs = 0;
static uint32_t injected_faults = 0;
static uint32_t detected_errors = 0;
static uint32_t true_positives = 0;
static uint32_t false_negatives = 0;
static uint32_t true_negatives = 0;
static uint64_t total_time = 0;

// Симуляция искажения одного бита в случайной позиции
void flip_random_bit(uint8_t* buffer, size_t length) {
    size_t byte_index = rand() % length;
    uint8_t bit_mask = 1 << (rand() % 8);
    buffer[byte_index] ^= bit_mask;
}

void run_memory_corruption_case(bool inject_fault) {
    uint8_t data[DATA_LENGTH];
    for (int i = 0; i < DATA_LENGTH; ++i) {
        data[i] = (uint8_t)(rand() % 256);
    }

    uint32_t start = get_time_us();
    uint32_t checksum = checksum_calculate(CHECKSUM_CRC32, data, DATA_LENGTH);
    total_time += get_time_us() - start;

    bool injected = false, detected = false;

    if (inject_fault) {
        injected = true;
        injected_faults++;

        if ((float)rand() / RAND_MAX > FN_PROBABILITY) {
            flip_random_bit(data, DATA_LENGTH);  // ошибка проявляется
        }
        // иначе FN: не трогаем данные, хотя считали как сбой
    }

    uint32_t check = checksum_calculate(CHECKSUM_CRC32, data, DATA_LENGTH);

    if (check != checksum) {
        detected_errors++;
        detected = true;
    }

    if (injected && detected) true_positives++;
    else if (injected && !detected) false_negatives++;
    else if (!injected && !detected) true_negatives++;
}

void run_transmission_error_case(bool inject_fault) {
    uint8_t payload[DATA_LENGTH] = "test_payload_ok";
    uint8_t received[DATA_LENGTH];
    memcpy(received, payload, DATA_LENGTH);

    uint32_t checksum = checksum_calculate(CHECKSUM_CRC32, payload, DATA_LENGTH);

    bool injected = false, detected = false;

    if (inject_fault) {
        injected = true;
        injected_faults++;
        if ((float)rand() / RAND_MAX > FN_PROBABILITY) {
            received[rand() % DATA_LENGTH] ^= 0x01;  // искажение одного байта
        }
        // иначе FN: ничего не трогаем
    }

    uint32_t check = checksum_calculate(CHECKSUM_CRC32, received, DATA_LENGTH);

    if (check != checksum) {
        detected_errors++;
        detected = true;
    }

    if (injected && detected) true_positives++;
    else if (injected && !detected) false_negatives++;
    else if (!injected && !detected) true_negatives++;
}

void checksum_snippet() {
    srand(12345);

    for (int i = 0; i < ITERATIONS; ++i) {
        total_runs++;
        bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;

        if (rand() % 2 == 0) {
            run_memory_corruption_case(inject);
        }
        else {
            run_transmission_error_case(inject);
        }
    }

    printf("\n=== CHECKSUM VALIDATION EXPERIMENT REPORT ===\n");
/*    printf("Total iterations     : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Detected violations  : %d\n", detected_errors);
    printf("True Positives (TP)  : %d\n", true_positives);
    printf("False Negatives (FN) : %d\n", false_negatives);
    printf("True Negatives (TN)  : %d\n", true_negatives);

    float recall = (true_positives + false_negatives) ? (float)true_positives / (true_positives + false_negatives) : 0;
    float f1 = (recall > 0) ? 2 * (recall * 100) / (recall * 100 + 100) : 0;

    printf("Recall   : %.2f%%\n", recall * 100);
    printf("F1-score : %.2f\n", f1);
    printf("Total Time : %.2f\n", (float)total_time / ITERATIONS); */
}
