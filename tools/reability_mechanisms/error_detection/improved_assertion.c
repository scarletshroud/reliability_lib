#include "improved_assertion.h"

assert_op_status_t reliability_assert_handler(const char* expr_str, const char* file, int line, assert_level_t level) {
    const char* level_str = (level == ASSERT_LEVEL_WARNING) ? "WARNING" : (level == ASSERT_LEVEL_ERROR) ? "ERROR" : "CRITICAL";

    // todo: to IO
    //fprintf(stderr, "[%s] Assertion failed: (%s), file: %s, line: %d\n", level_str, expr_str, file, line);

    // Дальнейшее поведение в зависимости от уровня
    switch (level) {
        case ASSERT_LEVEL_WARNING:
            // Можно просто логировать
            break;
        case ASSERT_LEVEL_ERROR:
            // Можно логировать и вернуться
            break;
        case ASSERT_LEVEL_CRITICAL:
            // Завершаем выполнение
            exit(1);
            break;
    }
}