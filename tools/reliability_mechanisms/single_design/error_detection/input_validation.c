#include <reliability_mechanisms/single_disign/error_detection/input_validation.h>
#include "error_monitor/error_monitor.h"

bool input_validate_field(const validator_t* validator, const void* value) {
    if (!validator || !validator->validate) {
        return false;
    }

    bool ok = validator->validate(value, validator->context);
    if (!ok) {
        error_monitor_save_event(
            __FILE__,
            validator->field_name,
            "Validation failed",
            __LINE__,
            ERROR_LEVEL_ERROR
        );
    }
    return ok;
}

bool input_validate_all(const validator_t* validators, uint32_t count, const void** values) {
    if (!validators || !values) {
        return false;
    }

    bool overall_ok = true;

    for (uint32_t i = 0; i < count; ++i) {
        if (!input_validate_field(&validators[i], values[i])) {
            overall_ok = false;
        }
    }

    return overall_ok;
}

// Проверка попадания числа в диапазон
typedef struct {
    int min;
    int max;
} range_context_t;

bool validate_range(const void* value, const void* context) {
    const int* val = (const int*)value;
    const range_context_t* range = (const range_context_t*)context;
    return (*val >= range->min) && (*val <= range->max);
}

// Проверка длины строки
typedef struct {
    uint32_t max_length;
} strlen_context_t;

bool validate_string_length(const void* value, const void* context) {
    const char* str = (const char*)value;
    const strlen_context_t* len = (const strlen_context_t*)context;
    uint32_t l = 0;
    while (str[l] != '\0') {
        l++;
        if (l > len->max_length) {
            return false;
        }
    }
    return true;
}