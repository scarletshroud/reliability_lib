#ifndef SYSTEM_BEHAVIOR_MONITOR_H
#define SYSTEM_BEHAVIOR_MONITOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        uint32_t last_kick_timestamp_us;    /* Последнее обновление сторожевого таймера */
        uint32_t timeout_us;                /* Установленный таймаут */
        const char* monitored_context;      /* Контекст мониторинга */
        uint8_t active;                     /* Признак активности WDT */
    } behavior_monitor_t;

    /**
     * @brief Инициализирует монитор поведения.
     *
     * Устанавливает начальное значение времени последнего "пинка", таймаут и контекст.
     * Также инициализирует сторожевой таймер платформы.
     *
     * @param[in,out] monitor Указатель на структуру монитора поведения.
     * @param[in] timeout_us Таймаут в микросекундах до срабатывания.
     * @param[in] context Строка с контекстом (например, имя задачи), для логирования ошибок.
    */
    void behavior_monitor_init(behavior_monitor_t* monitor, uint32_t timeout_us, const char* context);

    /**
     * @brief Запускает монитор поведения.
     *
     * Включает сторожевой таймер и активирует монитор.
     *
     * @param[in,out] monitor Указатель на структуру монитора поведения.
    */
    void behavior_monitor_start(behavior_monitor_t* monitor);
 
    /**
     * @brief Обновляет таймер "жизни" системы.
     *
     * Обновляет метку времени последнего "пинка" и сбрасывает сторожевой таймер,
     * если монитор активен.
     *
     * @param[in,out] monitor Указатель на структуру монитора поведения.
    */
    void behavior_monitor_kick(behavior_monitor_t* monitor);

    /**
     * @brief Проверяет, не вышел ли монитор за пределы таймаута.
     *
     * Если прошло больше времени, чем задано в `timeout_us`, сохраняется событие ошибки.
     *
     * @param[in] monitor Указатель на структуру монитора поведения.
     * @return 1, если система "жива" и таймаут не превышен; 0 в противном случае.
    */
    uint8_t behavior_monitor_check(const behavior_monitor_t* monitor);

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_BEHAVIOR_MONITOR_H
