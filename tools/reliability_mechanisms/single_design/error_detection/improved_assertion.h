#ifndef IMPROVED_ASSERTION_H_
#define IMPROVED_ASSERTION_H_

#include "tools_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Уровни важности для пользовательских утверждений (assert).
 */
typedef enum {
    ASSERT_LEVEL_DEBUG,    /**< Отладочный уровень. Поведение как у стандартного assert. */
    ASSERT_LEVEL_WARNING,  /**< Предупреждение: программа может продолжать работу. */
    ASSERT_LEVEL_ERROR,    /**< Ошибка: важно, но не обязательно фатально. */
    ASSERT_LEVEL_CRITICAL  /**< Критическая ошибка: программа/система может стать нестабильной. */
} assert_level_t;

/**
 * @brief Результат выполнения обработчика утверждения.
 */
typedef enum {
    ASSERT_OK  /**< Операция обработки завершилась успешно. */
} assert_op_status_t;

#if ENABLE_ASSERTS

/**
 * @brief Расширенная макро-функция ASSERT с уровнем важности.
 *
 * Проверяет выражение `expr` и вызывает обработчик `reliability_assert_handler`,
 * если выражение ложно. Работает только если `ENABLE_ASSERTS` включён.
 *
 * @param expr Выражение для проверки.
 * @param level Уровень важности ошибки (из `assert_level_t`).
 */
#define ASSERT(expr, level)                                                         \
    do {                                                                            \
        if (!(expr)) {                                                              \
            reliability_assert_handler(#expr, __FILE__, __LINE__, level);          \
        }                                                                           \
    } while (0)

#else

/// @brief Отключённый вариант ASSERT (ничего не делает при `ENABLE_ASSERTS == 0`).
#define ASSERT(expr, level) ((void)0)
#endif

/**
 * @brief Обработчик пользовательского утверждения (assert).
 *
 * Вызывается при ложном выражении в макросе `ASSERT`.
 *
 * @param expr_str Строковое представление выражения, не прошедшего проверку.
 * @param file Имя файла, где произошло утверждение.
 * @param line Номер строки в файле.
 * @param level Уровень важности утверждения.
 * @return Статус выполнения (всегда возвращает `ASSERT_OK`).
 */
assert_op_status_t reliability_assert_handler(const char* expr_str, const char* file, int line, assert_level_t level);

#ifdef __cplusplus
}
#endif

#endif // IMPROVED_ASSERTION_H_
