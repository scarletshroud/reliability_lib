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
     * \brief           Функция инициализация монитора поведения
     * \param[in]       monitor: Указатель на структуру монитора
     * \param[in]       timeout_us: Таймаут
     * \param[in]       context: Контекст монитора
    */
    void behavior_monitor_init(behavior_monitor_t* monitor, uint32_t timeout_us, const char* context);

    /**
     * \brief           Функция запуска монитора поведения
     * \param[in]       monitor: Указатель на структуру монитора
    */
    void behavior_monitor_start(behavior_monitor_t* monitor);
 
    /**
     * \brief           Функция подачи признака активности (kick)
     * \param[in]       monitor: Указатель на структуру монитора
    */
    void behavior_monitor_kick(behavior_monitor_t* monitor);

    /// Проверка срабатывания таймаута
    /**
     * \brief           Функция проверки срабатывания таймаута
     * \param[in]       monitor: Указатель на структуру монитора
    */
    uint8_t behavior_monitor_check(const behavior_monitor_t* monitor);

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_BEHAVIOR_MONITOR_H
