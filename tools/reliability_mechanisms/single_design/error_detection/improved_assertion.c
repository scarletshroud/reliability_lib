#include "improved_assertion.h"

#include <stdlib.h>
#include <stdint.h>

#ifdef ERROR_MONITOR_ENABLE 
#include "error_monitor/error_monitor.h"
#endif

#include "adapter/platform_adapter.h"

// todo: https://erdani.org/publications/cuj-08-2003.php.html полезный топик

// todo: добавить значения, которые использовались в expression для дополнения контекста
// todo: интеграция с IO
// todo: опция ignore
// todo: интеграция с release mode

static uint32_t assert_trigger_cnt = 0;

assert_op_status_t reliability_assert_handler(const char* expr_str, const char* file, int line, assert_level_t level) {
    const char* level_str = (level == ASSERT_LEVEL_WARNING) ? "WARNING" : (level == ASSERT_LEVEL_ERROR) ? "ERROR" : "CRITICAL";

    //fprintf(stderr, "[%s] Assertion failed: (%s), file: %s, line: %d\n", level_str, expr_str, file, line);

    assert_trigger_cnt++;

    // Дальнейшее поведение в зависимости от уровня
    switch (level) {
        case ASSERT_LEVEL_DEBUG:
            // ask the user what to do (Ignore/Debug/etc.)
            break;

        case ASSERT_LEVEL_WARNING:
            // Можно просто логировать и продолжить исполнение
            platform_print("Assert cought warning\n");
            break;

        case ASSERT_LEVEL_ERROR:
            // Можно логировать и вернуться, выбросить исключение

            platform_print("Assert cought error\n");

#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(file, "c", "assertion failed", line, ERROR_LEVEL_ERROR);
#endif

            break;

        case ASSERT_LEVEL_CRITICAL:
            // Завершаем выполнение

            // todo: Возможность перехода в безопасный режим
            // todo: __builtin_trap(); остановка под отладчиком
            // todo: поддержка пользовательского callback
            // todo: возможное создание дампа состояния

#ifdef  ERROR_MONITOR_ENABLE
            error_monitor_save_event(file, "c", "assertion failed", line, ERROR_LEVEL_CRITICAL);
#endif

            //exit(1);
            break;

        default:
            break;
    }
}
