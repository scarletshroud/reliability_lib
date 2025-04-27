#include "tests_model.h"

#include <stdlib.h>
#include <math.h>

void test_model_init(test_model_t *model, uint32_t num_errors) {
    model->num_errors = num_errors;
    model->intervals = (float *)malloc(sizeof(float) * num_errors);
    model->a = 0.0f;
    model->b = 0.0f;
    model->reliability = 1.0f;
}

void test_model_update_intervals(test_model_t *model, float *new_intervals) {
    for (uint32_t i = 0; i < model->num_errors; ++i) {
        model->intervals[i] = new_intervals[i];
    }
}

void test_model_estimate_parameters(test_model_t *model) {
    float sum_i2 = 0.0f;
    float sum_i4 = 0.0f;
    float sum_N = 0.0f;
    float sum_i2N = 0.0f;

    for (uint32_t i = 0; i < model->num_errors; ++i) {
        float idx = (float)(i + 1);
        float idx2 = idx * idx;
        float N = model->intervals[i];

        sum_i2 += idx2;
        sum_i4 += idx2 * idx2;
        sum_N += N;
        sum_i2N += idx2 * N;
    }

    float denominator = (model->num_errors * sum_i4) - (sum_i2 * sum_i2);
    if (denominator != 0.0f) {
        model->a = ((sum_i4 * sum_N) - (sum_i2 * sum_i2N)) / denominator;
        model->b = ((model->num_errors * sum_i2N) - (sum_i2 * sum_N)) / denominator;
    } else {
        model->a = 0.0f;
        model->b = 0.0f;
    }
}

void test_model_calculate_reliability(test_model_t *model) {
    float total_tests = 0.0f;
    for (uint32_t i = 0; i < model->num_errors; ++i) {
        total_tests += model->intervals[i];
    }
    float expected_errors = 0.0f;
    for (uint32_t i = 0; i < model->num_errors; ++i) {
        float idx = (float)(i + 1);
        expected_errors += 1.0f / (model->a + model->b * idx * idx);
    }
    model->reliability = 1.0f - (model->num_errors / expected_errors);
}