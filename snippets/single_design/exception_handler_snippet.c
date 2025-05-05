#include "exception_handler_snippet.h"

#include "tools/reliability_mechanisms/single_design/exception_handler.h"

#include "adapter/platform_adapter.h"

// Пользовательский обработчик
void my_exception_handler(ExceptionType ex_type, const char* file, const char* func, uint32_t line) {
    platform_print("[EXCEPTION] Catched.");

    switch (ex_type) {
        case EXCEPTION_ASSERTION_FAILED:
            // Можно, например, завершить задачу
            break;

        case EXCEPTION_DEADLINE_EXCEEDED:
            // Перезапустить задачу или перейти в безопасный режим
            break;

        case EXCEPTION_MEMORY_CORRUPTION:
            // Немедленный reset системы
            break;

        case EXCEPTION_DIVISION_BY_ZERO:
            break;

        default:
            break;
    }
}

void some_critical_function() {
    int divisor = 0;

    if (divisor == 0) {
        EXCEPTION_RAISE(EXCEPTION_DIVISION_BY_ZERO);
        return;
    }

    int result = 100 / divisor;
}

void exception_handler_snippet(void) {
    exception_handler_init(my_exception_handler);

    some_critical_function();
}
