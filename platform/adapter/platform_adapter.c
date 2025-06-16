#include "platform_adapter.h"

#include "platform_config.h"

#ifdef TARGET_QEMU_RISCV
#include "qemu/qemu_riscv_uart.h"
#endif

#ifdef TARGET_MK32
#include "xprintf.h"
#include "uart_lib.h"
#include "mk32_system.h"
#endif 

static platform_adapter_t adapter;

void adapter_init(adapter_target_t target) {
    //доделать инициализацию

    adapter.target = target;
}

void platform_init() {

#ifdef TARGET_QEMU_RISCV
    qemu_riscv_uart_enable();
#endif

#ifdef TARGET_MK32
    mk32_system_init();
#endif

}

void platform_print(const char *str) {
#ifdef TARGET_QEMU_RISCV
    qemu_riscv_uart_puts(str);
#endif

#ifdef TARGET_MK32
    xprintf(str);
#endif
}

uint32_t platform_timer_get_us() {
#ifdef TARGET_MK32
    return mk32_get_time_us();
#endif
}

/* WDT */

void platform_wdt_init(uint32_t timeout) {
#ifdef TARGET_MK32
  
#endif
}

void platform_wdt_start() {
#ifdef TARGET_MK32
  
#endif
}

void platform_wdt_kick() {
#ifdef TARGET_MK32
  
#endif
}

