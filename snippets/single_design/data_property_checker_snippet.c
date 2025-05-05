#include "data_property_checker_snippet.h"

#include "tools/reliability_mechanisms/single_design/error_detection/data_property_checker.h"

typedef struct {
    uint32_t length;
    uint32_t capacity;
    uint8_t* buffer;
} MyBuffer;

bool check_length_vs_capacity(const void* structure) {
    const MyBuffer* buf = (const MyBuffer*)structure;
    return (buf->length <= buf->capacity);
}

bool check_buffer_not_null(const void* structure) {
    const MyBuffer* buf = (const MyBuffer*)structure;
    return (buf->buffer != 0);
}

void data_property_checker_snippet() {
    MyBuffer buf = {
        .length = 10,
        .capacity = 20,
        .buffer = 0 // ОШИБКА: указатель не инициализирован
    };

    property_checker_t checkers[] = {
        { "Length <= Capacity", check_length_vs_capacity },
        { "Buffer pointer not NULL", check_buffer_not_null }
    };

    bool ok = data_property_check_all(checkers, 2, &buf);

    if (!ok) {

    } else {

    }
}