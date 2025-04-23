#include "shuman_based.h"

#include <math.h>
#include <stddef.h>

static void binary_search() {

}

static float define_errors_intensity(const shuman_based_model_t* model, uint32_t defects_cnt) {

}

uint8_t shuman_collect_reability_stats(const shuman_based_model_t* model, shuman_based_model_stats_t* stats) {
    uint32_t n = 0;
    uint32_t errors_cnt = 0;
    uint32_t defects_cnt = 0;
    uint32_t tests_duration = 0;

    float sum = 0;
    float left_equation = 0.0f;
    float right_equation = 0.0f;

    if (model == NULL || stats == NULL) {
        return 1;
    }

    for (uint32_t param = 0; param < 100; ++param) {
        for (uint32_t i = 0; i < model->tests_cnt; ++i) {
            n += model->test_errors_cnt[i];
            errors_cnt += model->test_errors_cnt[i];
            tests_duration += model->test_duration[i];
        }

        for (uint32_t i = 0; i < model->tests_cnt; ++i) {
            uint32_t current_n = 0;
            for (uint32_t j = 0; j < i; ++j) {
                current_n += 1;
            }

            right_equation += (defects_cnt - current_n) * model->test_duration[i];
        }

        for (uint32_t i = 0; i < model->tests_cnt; ++i) {
            uint32_t current_n = 0;
            for (uint32_t j = 0; j < i; ++j) {
                current_n += 1;
            }

            sum += model->test_errors_cnt[i] / (defects_cnt - current_n);
        }

        left_equation = errors_cnt * tests_duration / sum;

        if (fabs(left_equation - right_equation) < 1) {
            stats->defects_cnt = defects_cnt;
            break;
        }
    }

    stats->fault_intensity = (sum / tests_duration) * (defects_cnt - n);
    stats->p_correct_exec = (1 - stats->fault_intensity / model->req_intensity); // todo: вероятность сбойных ошибок
    stats->p_error_free = exp(-stats->fault_intensity * model->specified_time);
    stats->mtbf = 1 / (model->req_intensity * (1 - stats->p_error_free));
    stats->p_fault_free = exp(-model->sys_fault_intensity * model->specified_time) * (1 - (1 - model->p_fault_detect * model->p_fault_process) * model->p_fault_transform * (1 - stats->p_error_free));
    stats->mtbf_t = (1 - model->p_fault_transform * (1 + model->p_fault_detect * model->p_fault_process)) / model->sys_fault_intensity + (model->p_fault_transform * (1 + model->p_fault_detect * model->p_fault_process) / (stats->fault_intensity * model->sys_fault_intensity));
    stats->k_ready = stats->mtbf / (stats->mtbf + model->t_sys_recovery);

    return 0;
}
