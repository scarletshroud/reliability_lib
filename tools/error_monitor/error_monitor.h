#ifndef ERROR_MONITOR_H_
#define ERROR_MONITOR_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Максимальное количество записей в журнале ошибок.
 */
#define ERROR_LOG_CAPACITY 256

/**
 * @brief Уровень серьёзности ошибки.
 */
typedef enum {
    ERROR_LEVEL_INFO = 0,      /**< Информационное сообщение. */
    ERROR_LEVEL_WARNING,       /**< Предупреждение. */
    ERROR_LEVEL_ERROR,         /**< Ошибка, требующая внимания. */
    ERROR_LEVEL_CRITICAL       /**< Критическая ошибка, угрожающая стабильности системы. */
} error_level_t;

/**
 * @brief Структура, описывающая одно событие ошибки.
 */
typedef struct {
    uint32_t timestamp;           /**< Метка времени возникновения ошибки (в мс или мкс). */
    uint32_t line;                /**< Номер строки исходного файла, где произошла ошибка. */
    const char* file;             /**< Имя исходного файла. */
    const char* module;           /**< Идентификатор модуля/контекста, вызвавшего ошибку. */
    const char* message;          /**< Описание ошибки. */
    error_level_t error_level;    /**< Уровень серьёзности ошибки. */
} error_event_t;

/**
 * @brief Статус выполнения операций с журналом ошибок.
 */
typedef enum {
    ERROR_MONITOR_OP_OK = 0,      /**< Операция завершена успешно. */
    ERROR_MONITOR_OVERFLOW        /**< Буфер журналирования переполнен. */
} error_monitor_op_status_t;

/**
 * @brief Сохраняет событие ошибки в журнал.
 *
 * Добавляет запись в кольцевой буфер с информацией о месте и характере ошибки.
 *
 * @param[in] file Имя файла, где была зафиксирована ошибка (обычно __FILE__).
 * @param[in] module Идентификатор модуля/контекста (например, имя задачи или компонента).
 * @param[in] message Строка с сообщением об ошибке.
 * @param[in] line Номер строки, где произошла ошибка (обычно __LINE__).
 * @param[in] error_level Уровень серьёзности ошибки.
 * @return Статус операции: `ERROR_MONITOR_OP_OK` или `ERROR_MONITOR_OVERFLOW`.
 */
error_monitor_op_status_t error_monitor_save_event(
    const char* file,
    const char* module,
    const char* message,
    uint32_t line,
    error_level_t error_level
);

/**
 * @brief Очищает журнал ошибок.
 *
 * Удаляет все сохранённые события и сбрасывает счётчики.
 */
void error_monitor_clear(void);

/**
 * @brief Возвращает количество зарегистрированных ошибок (всех уровней).
 *
 * @return Число записей об ошибках, хранящихся в журнале.
 */
uint32_t error_monitor_err_cnt(void);

// TODO: добавить доступ к последним событиям через итератор или указатель
// const error_event_t* error_monitor_get_recent();

#ifdef __cplusplus
}
#endif

#endif // ERROR_MONITOR_H_
