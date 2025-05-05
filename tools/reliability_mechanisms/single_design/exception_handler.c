#include "exception_handler.h"

#include "error_monitor/error_monitor.h" // Чтобы также логировать ошибки

static exception_handler_fn user_handler = 0;

void exception_handler_init(exception_handler_fn handler) {
    user_handler = handler;
}

void exception_raise(ExceptionType ex_type, const char* file, const char* func, uint32_t line) {
    // Сначала фиксируем в error_monitor
    error_monitor_save_event(file, func, "Exception occurred", line, ERROR_LEVEL_CRITICAL);

    // Затем, если зарегистрирован пользовательский обработчик, вызвать его
    if (user_handler) {
        user_handler(ex_type, file, func, line);
    }
}