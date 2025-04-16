#include "init_platform_adapter.h"

#include "qemu_riscv_uart.h"

void init_target(adapter_target_t target) {
    switch (target) {
        case ADAPTER_TARGET_QEMU_RISCV:
            qemu_riscv_uart_enable();
            break;

        default:
            break;
    } 
}