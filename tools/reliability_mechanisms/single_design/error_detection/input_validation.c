#include "input_validation.h"

#include "tools/tools_config.h"
#include "error_monitor/error_monitor.h"

uint8_t input_validate_field(const validator_t* validator, const void* value) {
    if (!validator || !validator->validate) {
        return 0;
    }

    uint8_t ok = validator->validate(value, validator->context);
    if (!ok) {

#ifdef ERROR_MONITOR_ENABLE
        error_monitor_save_event(
            __FILE__,
            validator->field_name,
            "Validation failed",
            __LINE__,
            ERROR_LEVEL_ERROR
        );
#endif

    }
    return ok;
}

uint8_t input_validate_all(const validator_t* validators, uint32_t count, const void** values) {
    if (!validators || !values) {
        return 0;
    }

    uint8_t overall_ok = 1;

    for (uint32_t i = 0; i < count; ++i) {
        if (!input_validate_field(&validators[i], values[i])) {
            overall_ok = 0;
        }
    }

    return overall_ok;
}

// Проверка попадания числа в диапазон
typedef struct {
    int min;
    int max;
} range_context_t;

uint8_t validate_range(const void* value, const void* context) {
    const int* val = (const int*)value;
    const range_context_t* range = (const range_context_t*)context;
    return (*val >= range->min) && (*val <= range->max);
}

// Проверка длины строки
typedef struct {
    uint32_t max_length;
} strlen_context_t;

uint8_t validate_string_length(const void* value, const void* context) {
    const char* str = (const char*)value;
    const strlen_context_t* len = (const strlen_context_t*)context;
    uint32_t l = 0;
    while (str[l] != '\0') {
        l++;
        if (l > len->max_length) {
            return 0;
        }
    }
    return 1;
}
