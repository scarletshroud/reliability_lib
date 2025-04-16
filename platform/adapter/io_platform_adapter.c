#include "io_platform_adapter.h"

#include "qemu_riscv_uart.h"

void print(const char *str, adapter_target_t target) {
    switch (target) {
        case ADAPTER_TARGET_QEMU_RISCV:
            qemu_riscv_uart_puts(str);
            break;

        default:
            break;
    }
}