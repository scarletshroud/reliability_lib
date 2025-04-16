#include <stdint.h>

#include "platform/adapter/platform_adapter.h"
#include "snippets/assert_snippet.h"
#include "tools/error_monitor/error_monitor.h"

void run_snippet(const char* label, void (*snippet)(void), uint32_t steps) {
    error_monitor_clear();

    for (uint32_t i = 0; i < steps; ++i) {
        uint32_t err_cnt_before = 0;
        uint32_t err_cnt_after = 0;
        uint32_t err_cnt_new = 0;

        platform_print(label);

        err_cnt_before = error_monitor_err_cnt();

        snippet();

        err_cnt_after = error_monitor_err_cnt();

        err_cnt_new = (err_cnt_after > err_cnt_before) ? (err_cnt_after - err_cnt_before) : 0;

        // добавляем данные о шаге в модель
    }
}

int main() {
    /* System init */
    adapter_init(ADAPTER_TARGET_QEMU_RISCV);
    platform_init();

    /* Run snippets */
    run_snippet("No assertion", &no_assert_snippet, 100);
    run_snippet("With assertion", &assert_snippet, 100);

    /* Evaluate the effect of reliability mechanism integrity */
    

    return 0;
}