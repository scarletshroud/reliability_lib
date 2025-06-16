#ifndef RECOVERY_BLOCK_H
#define RECOVERY_BLOCK_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Максимальное количество вариантов (версий) в восстановительном блоке.
 */
#define MAX_RB_VARIANTS 4

/**
 * @brief Тип функции-варианта восстановления.
 *
 * Каждая функция-реализация получает входные данные и сохраняет результат.
 *
 * @param[out] result Указатель на буфер для результата.
 * @param[in] input Указатель на входные данные.
 * @return true, если выполнение прошло успешно.
 */
typedef bool (*rb_variant_fn_t)(void* result, const void* input);

/**
 * @brief Тип функции приёмочного теста.
 *
 * Используется в режиме SEQUENTIAL для проверки корректности результата.
 *
 * @param[in] result Результат, подлежащий проверке.
 * @return true, если результат проходит приёмочный тест.
 */
typedef bool (*rb_acceptance_test_fn_t)(const void* result);

/**
 * @brief Тип функции сравнения результатов.
 *
 * Используется в режиме CONSENSUS для определения совпадающих результатов.
 *
 * @param[in] result1 Первый результат.
 * @param[in] result2 Второй результат.
 * @return true, если результаты считаются эквивалентными.
 */
typedef bool (*rb_compare_fn_t)(const void* result1, const void* result2);

/**
 * @brief Тип функции копирования результата.
 *
 * Применяется для копирования выбранного надёжного результата в CONSENSUS-режиме.
 *
 * @param[out] dest Буфер-приёмник.
 * @param[in] src Буфер-источник.
 */
typedef void (*rb_copy_fn_t)(void* dest, const void* src);

/**
 * @brief Режим работы восстановительного блока.
 */
typedef enum {
    RB_MODE_SEQUENTIAL, /**< Последовательный режим: варианты исполняются поочерёдно, пока результат не пройдет приёмочный тест. */
    RB_MODE_CONSENSUS   /**< Режим консенсуса: все варианты исполняются, сравниваются, и выбирается согласованный результат. */
} rb_mode_t;

/**
 * @brief Структура, описывающая параметры восстановительного блока.
 */
typedef struct {
    rb_variant_fn_t variants[MAX_RB_VARIANTS];        /**< Массив функций-вариантов. */
    uint32_t variant_count;                           /**< Количество доступных вариантов. */
    rb_acceptance_test_fn_t acceptance_test;          /**< Приёмочный тест (для SEQUENTIAL). */
    rb_compare_fn_t consensus_compare;                /**< Функция сравнения результатов (для CONSENSUS). */
    rb_copy_fn_t consensus_copy;                      /**< Функция копирования результата (для CONSENSUS). */
    rb_mode_t mode;                                   /**< Режим работы блока. */
    const char* context;                              /**< Контекст (строка идентификатор, используется для отладки/логирования). */
} recovery_block_t;

/**
 * @brief Выполняет восстановительный блок по заданной конфигурации.
 *
 * В зависимости от режима выполняет либо последовательный перебор с приёмочным тестом,
 * либо параллельное исполнение и поиск консенсуса по результатам.
 *
 * @param[in] rb Указатель на структуру блока восстановления.
 * @param[in] input Входные данные.
 * @param[out] result Буфер для записи выбранного (надежного) результата.
 * @return true, если удалось выбрать корректный результат, иначе false.
 */
bool recovery_block_execute(
    const recovery_block_t* rb,
    const void* input,
    void* result
);

#ifdef __cplusplus
}
#endif

#endif // RECOVERY_BLOCK_H
