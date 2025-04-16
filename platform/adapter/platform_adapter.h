#ifndef PLATFORM_ADAPTER_H_
#define PLATFORM_ADAPTER_H_

#include "adapter_target.h"

typedef struct {
    adapter_target_t target;
} platform_adapter_t;

void adapter_init(adapter_target_t target);

void platform_init();
void platform_print(const char *str);

#endif