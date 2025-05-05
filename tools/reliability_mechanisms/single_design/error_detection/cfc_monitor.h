#ifndef CFC_MONITOR_H
#define CFC_MONITOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CFC_ENTER(monitor, sig) cfc_monitor_enter(monitor, sig)
#define CFC_CHECK(monitor, expected_sig) cfc_monitor_check(monitor, expected_sig, __func__)

    typedef struct {
        uint32_t signature;      // Текущая сигнатура
    } cfc_monitor_t;

    /// Инициализация CFC-монитора
    void cfc_monitor_init(cfc_monitor_t* monitor);

    /// Установить сигнатуру при входе в базовый блок
    void cfc_monitor_enter(cfc_monitor_t* monitor, uint32_t block_signature);

    /// Проверить сигнатуру при переходе в следующий базовый блок
    bool cfc_monitor_check(cfc_monitor_t* monitor, uint32_t expected_signature, const char* context);

#ifdef __cplusplus
}
#endif

#endif // CFC_MONITOR_H
