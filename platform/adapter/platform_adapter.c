#include "platform_adapter.h"

#include "qemu_riscv_uart.h"

static platform_adapter_t adapter;

void adapter_init(adapter_target_t target) {
    //доделать инициализацию

    adapter.target = target;
}

void platform_init() {
    switch (adapter.target) {
        case ADAPTER_TARGET_QEMU_RISCV:
            qemu_riscv_uart_enable();
            break;

        default:
            break;
    } 
}

void platform_print(const char *str) {
    switch (adapter.target) {
        case ADAPTER_TARGET_QEMU_RISCV:
            qemu_riscv_uart_puts(str);
            break;

        default:
            break;
    }
}