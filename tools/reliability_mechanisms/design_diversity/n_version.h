#ifndef N_VERSION_EXEC_H
#define N_VERSION_EXEC_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Максимальное количество версий функции, поддерживаемое системой.
 */
#define MAX_N_VERSIONS 5

/**
 * @brief Тип функции-версии.
 *
 * Каждая версия принимает входные данные и возвращает результат через указатель.
 *
 * @param[out] result Указатель на буфер для записи результата.
 * @param[in] input Указатель на входные данные.
 * @return true, если функция успешно выполнилась.
 */
typedef bool (*n_version_fn_t)(void* result, const void* input);

/**
 * @brief Тип функции сравнения результатов двух версий.
 *
 * @param[in] r1 Указатель на результат первой версии.
 * @param[in] r2 Указатель на результат второй версии.
 * @return true, если результаты считаются эквивалентными.
 */
typedef bool (*n_version_compare_fn_t)(const void* r1, const void* r2);

/**
 * @brief Тип функции копирования результата.
 *
 * Используется для копирования выбранного корректного результата в выходной буфер.
 *
 * @param[out] dst Назначение (буфер).
 * @param[in] src Источник (результат одной из версий).
 */
typedef void (*n_version_copy_fn_t)(void* dst, const void* src);

/**
 * @brief Режим голосования для выбора корректного результата.
 */
typedef enum {
    N_VERSION_VOTING_STRICT,    /**< Строгое голосование: все версии должны совпадать. */
    N_VERSION_VOTING_MAJORITY,  /**< Мажоритарное голосование: выбирается наиболее часто встречающийся результат. */
    N_VERSION_VOTING_THRESHOLD  /**< Голосование по порогу: результат принимается, если набрано не менее threshold совпадений. */
} n_version_voting_mode_t;

/**
 * @brief Структура конфигурации для многоверсионного исполнения.
 */
typedef struct {
    n_version_fn_t versions[MAX_N_VERSIONS];     /**< Массив указателей на версии функций. */
    uint32_t version_count;                      /**< Количество зарегистрированных версий. */
    uint32_t threshold;                          /**< Порог голосования (используется в режиме THRESHOLD). */
    n_version_compare_fn_t compare;              /**< Функция сравнения результатов. */
    n_version_copy_fn_t copy;                    /**< Функция копирования результата. */
    n_version_voting_mode_t mode;                /**< Режим голосования. */
    const char* context;                         /**< Строка с описанием контекста (для отладки/логирования). */
} n_version_exec_t;

/**
 * @brief Выполняет все зарегистрированные версии функции и выбирает корректный результат.
 *
 * Использует заданный режим голосования (`mode`) и функцию сравнения, чтобы определить
 * большинство или совпадающий результат. Выбранный результат копируется в `selected_output`.
 *
 * @param[in] exec Указатель на структуру конфигурации.
 * @param[in] input Входные данные.
 * @param[out] result_buffers Массив буферов, по одному на каждую версию, для записи результатов.
 * @param[out] selected_output Буфер, в который будет записан выбранный (надежный) результат.
 * @return true, если удалось выбрать корректный результат согласно политике голосования.
 */
bool n_version_execute(
    const n_version_exec_t* exec,
    const void* input,
    void** result_buffers,
    void* selected_output
);

#ifdef __cplusplus
}
#endif

#endif // N_VERSION_EXEC_H
