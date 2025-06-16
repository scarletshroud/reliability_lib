#ifndef PROCESS_PAIR_H
#define PROCESS_PAIR_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Режим работы процессной пары.
 *
 * Определяет, как исполняются основная и резервная функции обработки.
 */
typedef enum {
    PROCESS_PAIR_ACTIVE_PASSIVE, /**< Выполняется только активная функция; резервная — используется при необходимости. */
    PROCESS_PAIR_ACTIVE_ACTIVE   /**< Выполняются обе функции параллельно и сравниваются результаты. */
} process_pair_mode_t;

/**
 * @brief Тип функции обработки.
 *
 * Пользовательская функция, принимающая входные данные и записывающая результат.
 *
 * @param[out] result Буфер, в который помещается результат обработки.
 * @param[in] input Входные данные.
 * @return true, если обработка прошла успешно, иначе false.
 */
typedef bool (*process_fn_t)(void* result, const void* input);

/**
 * @brief Тип функции сравнения результатов.
 *
 * Функция сравнивает два результата и сообщает, идентичны ли они.
 *
 * @param[in] result1 Первый результат.
 * @param[in] result2 Второй результат.
 * @return true, если результаты эквивалентны, иначе false.
 */
typedef bool (*process_compare_fn_t)(const void* result1, const void* result2);

/**
 * @brief Структура, описывающая конфигурацию процессной пары.
 */
typedef struct {
    process_fn_t active;                  /**< Основная функция обработки. */
    process_fn_t backup;                  /**< Резервная функция обработки. */
    process_compare_fn_t compare;         /**< Функция сравнения результатов (используется в режиме Active-Active). */
    process_pair_mode_t mode;            /**< Режим выполнения: Active-Passive или Active-Active. */
    const char* context;                 /**< Описание/идентификатор контекста, используемый для логирования. */
} process_pair_t;

/**
 * @brief Выполняет обработку входных данных с использованием процессной пары.
 *
 * В зависимости от режима вызывает активную и/или резервную функцию, а также сравнивает результаты.
 *
 * @param[in] pair Указатель на структуру конфигурации процессной пары.
 * @param[in] input Входные данные для обработки.
 * @param[out] result_main Результат работы основной функции.
 * @param[out] result_backup Результат работы резервной функции (если применимо).
 * @return true, если выполнение прошло успешно и сравнение (при необходимости) прошло без расхождений.
 */
bool process_pair_execute(
    const process_pair_t* pair,
    const void* input,
    void* result_main,
    void* result_backup
);

#ifdef __cplusplus
}
#endif

#endif // PROCESS_PAIR_H
