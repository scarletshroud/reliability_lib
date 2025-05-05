#include "cfc_monitor_snippet.h"

#include "tools/reliability_mechanisms/single_design/error_detection/cfc_monitor.h"

cfc_monitor_t monitor;

void cfc_monitor_snippet() {
    cfc_monitor_init(&monitor);

    CFC_ENTER(&monitor, 0x1234); // Входим в первый блок

    // Выполняем код блока 1

    CFC_CHECK(&monitor, 0x1234); // Проверяем сигнатуру после блока 1

    CFC_ENTER(&monitor, 0x5678); // Входим во второй блок

    // Выполняем код блока 2

    CFC_CHECK(&monitor, 0x5678); // Проверяем сигнатуру после блока 2
}