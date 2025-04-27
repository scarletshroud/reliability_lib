#include <stdint.h>

#include "snippets/assert_snippet.h"
#include "tools/error_monitor/error_monitor.h"
#include "tools/reliability_evaluation_models/discrete/shuman_based.h"

#include "platform/adapter/platform_adapter.h"
#include "platform/mk32_baremetal/src/mk32_system.h"

static void run_snippet(const char* label, void (*snippet)(void), uint32_t steps) {
    error_monitor_clear();

    platform_print(label);

	uint32_t err_cnt_before = 0;
	uint32_t err_cnt_after = 0;
	uint32_t err_cnt_new = 0;

	err_cnt_before = error_monitor_err_cnt();

	for (uint32_t i = 0; i < steps; ++i) {
        snippet();
	}

	err_cnt_after = error_monitor_err_cnt();

	err_cnt_new = (err_cnt_after > err_cnt_before) ? (err_cnt_after - err_cnt_before) : 0;

        // добавляем данные о шаге в модель
}

int main() {
    /* System init */
    platform_init();
    platform_print("System was successfully initialized.\n");

    /* Run snippets */
    run_snippet("Snippet without assertion\n", &no_assert_snippet, 100);
    run_snippet("Snippet without assertion\n", &assert_snippet, 100);

    /* Evaluate the effect of reliability mechanism integrity */

    shuman_based_model_t info; 
    shuman_based_model_stats_t stats; 

    info.tests_cnt = 10;
    info.req_intensity = 3600;
    info.p_fault_transform = 0.01f;
    info.sys_fault_intensity = 0.000001f;
    info.p_fault_detect = 0.99f;
    info.p_fault_process = 0.95f;
    info.t_sys_recovery = 2.5;
    info.specified_time = 8;

    info.test_duration[0] = 12;
    info.test_duration[1] = 11;
    info.test_duration[2] = 12;
    info.test_duration[3] = 8;
    info.test_duration[4] = 10;
    info.test_duration[5] = 11;
    info.test_duration[6] = 12;
    info.test_duration[7] = 13;
    info.test_duration[8] = 9;
    info.test_duration[9] = 10;

    info.test_errors_cnt[0] = 2;
    info.test_errors_cnt[1] = 3;
    info.test_errors_cnt[2] = 1;
    info.test_errors_cnt[3] = 0;
    info.test_errors_cnt[4] = 0;
    info.test_errors_cnt[5] = 1;
    info.test_errors_cnt[6] = 2;
    info.test_errors_cnt[7] = 0;
    info.test_errors_cnt[8] = 0;
    info.test_errors_cnt[9] = 1;

    info.p_fault = 1;

    shuman_collect_reability_stats(&info, &stats);

    return 0;
}
