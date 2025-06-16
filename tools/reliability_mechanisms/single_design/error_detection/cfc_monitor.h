#ifndef CFC_MONITOR_H
#define CFC_MONITOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Макросы для использования CFC-монитора */
#define CFC_ENTER(monitor, sig) cfc_monitor_enter(monitor, sig)
#define CFC_CHECK(monitor, expected_sig) cfc_monitor_check(monitor, expected_sig, __func__)

    typedef struct {
        uint32_t signature;      /* Текущая сигнатура */
    } cfc_monitor_t;

    /**
     * @brief           Функция инициализации CFC-монитора
     * @param[in]       monitor: Указатель на структуру монитора
    */
    void cfc_monitor_init(cfc_monitor_t* monitor);

    /**
     * @brief           Функция установки сигнатуры при входе в базовый блок
     * @param[in]       monitor: Указатель на структуру монитора
     * @param[in]       block_signature: Сигнатура блока
    */
    void cfc_monitor_enter(cfc_monitor_t* monitor, uint32_t block_signature);

    /**
     * @brief           Функция проверки сигнатуры при переходе в следующий базовый блок
     * @param[in]       monitor:  Указатель на структуру монитора
     * @param[in]       expected_signature: Ожидаемая сигнатура
     * @param[in]       context: Указатель на дополнительный контекст для вывода сообщений
     * @return          Результат проверки: 0 - ошибка, 1 - успешно
    */
    uint8_t cfc_monitor_check(cfc_monitor_t* monitor, uint32_t expected_signature, const char* context);

#ifdef __cplusplus
}
#endif

#endif // CFC_MONITOR_H
