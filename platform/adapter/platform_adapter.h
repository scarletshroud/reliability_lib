#ifndef PLATFORM_ADAPTER_H_
#define PLATFORM_ADAPTER_H_

#include <stdint.h>

#include "adapter_target.h"

typedef struct {
    adapter_target_t target;
} platform_adapter_t;

void adapter_init(adapter_target_t target);

void platform_init();
void platform_print(const char *str);
uint32_t platform_timer_get_us();

/* WDT */
void platform_wdt_init(uint32_t timeout);
void platform_wdt_start();
void platform_wdt_kick();

#endif
