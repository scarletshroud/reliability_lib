#include "checkpoint_restart_snippet.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "reliability_mechanisms/single_design/checkpoint_restart.h"
#include "error_monitor/error_monitor.h"

#include "platform/adapter/platform_adapter.h"

#define ITERATIONS 1000
#define ERROR_PROBABILITY 0.2     // Вероятность инъекции ошибки
#define FN_PROBABILITY 0.01       // Вероятность "маскировки" ошибки

// Структура, имитирующая внутреннее состояние системы
typedef struct {
    int pos;
    int vel;
    int error_counter;
} SystemState;

// Статистика выполнения
static int total_runs = 0;
static int injected_faults = 0;
static int successful_recovery = 0;
static int failed_recovery = 0;
static int masked_errors = 0;

/**
 * @brief Инъецирует ошибку в состояние, если задан флаг inject_fault.
 *
 * Ошибка считается "замаскированной", если значение не попало под ожидаемое.
 */
static void corrupt_state(SystemState* state, bool inject_fault) {
    if (inject_fault) {
        injected_faults++;
        // Только если "ошибка" не замаскирована — она задаёт отличительное значение
        if ((float)rand() / RAND_MAX > FN_PROBABILITY) {
            state->error_counter = 999;
        }
    }
}

/**
 * @brief Выполняет тест со статической контрольной точкой.
 *
 * Контрольная точка создаётся один раз до повреждения, а затем используется для восстановления.
 */
static void run_static_checkpoint_case() {
    // Исходное состояние системы
    SystemState current = { 0, 1, 0 };

    // Копия состояния сохраняется в качестве статической контрольной точки
    SystemState static_ref;
    memcpy(&static_ref, &current, sizeof(SystemState));

    checkpoint_t static_cp = {
        .state_buffer = &static_ref,
        .size = sizeof(SystemState)
    };

    // Решаем, будет ли инъекция ошибки
    bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;
    corrupt_state(&current, inject);

    // Если ошибка действительно была инъецирована:
    if (inject) {
        if (current.error_counter == 999) {
            // Пытаемся восстановить состояние из контрольной точки
            bool restored = checkpoint_static_restart(&static_cp, &current);
            if (restored) successful_recovery++;
            else failed_recovery++;
        } else {
            // Ошибка была замаскирована — система не распознала её как неисправность
            masked_errors++;
        }
    }
}

/**
 * @brief Выполняет тест с динамически сохраняемой контрольной точкой.
 *
 * Контрольная точка создаётся через вызов `checkpoint_dynamic_save` во время выполнения.
 */
static void run_dynamic_checkpoint_case() {
    SystemState current = { 0, 2, 0 };         // Текущее состояние системы
    SystemState dynamic_buf = { 0 };          // Буфер для сохранения состояния

    checkpoint_t dyn_cp = {
        .state_buffer = &dynamic_buf,
        .size = sizeof(SystemState)
    };

    // Сохраняем контрольную точку динамически
    checkpoint_dynamic_save(&dyn_cp, &current);

    // Инъекция ошибки (возможно)
    bool inject = ((float)rand() / RAND_MAX) < ERROR_PROBABILITY;
    corrupt_state(&current, inject);

    // Анализ результата восстановления
    if (inject) {
        if (current.error_counter == 999) {
            bool restored = checkpoint_dynamic_restore(&dyn_cp, &current);
            if (restored) successful_recovery++;
            else failed_recovery++;
        } else {
            masked_errors++;
        }
    }
}

/**
 * @brief Главная функция эксперимента: выполняет тесты восстановления как со статическими,
 * так и с динамическими контрольными точками.
 *
 * После каждой серии тестов отображает отчёт (вывод закомментирован).
 */
void checkpoint_restart_snippet(void) {
    srand(12345);

    platform_print("\n=== STATIC CHECKPOINT TEST ===\n");

    for (int i = 0; i < ITERATIONS; i++) {
        total_runs++;
        run_static_checkpoint_case();
    }

    /*
    printf("Total runs           : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Recovered (success)  : %d\n", successful_recovery);
    printf("Recovery failed      : %d\n", failed_recovery);
    printf("Masked errors        : %d\n", masked_errors);
    */

    // Обнуление счётчиков перед динамическим тестом
    total_runs = injected_faults = successful_recovery = failed_recovery = masked_errors = 0;

    platform_print("\n=== DYNAMIC CHECKPOINT TEST ===\n");

    for (int i = 0; i < ITERATIONS; i++) {
        total_runs++;
        run_dynamic_checkpoint_case();
    }

    /*
    printf("Total runs           : %d\n", total_runs);
    printf("Injected faults      : %d\n", injected_faults);
    printf("Recovered (success)  : %d\n", successful_recovery);
    printf("Recovery failed      : %d\n", failed_recovery);
    printf("Masked errors        : %d\n", masked_errors);
    */
}
