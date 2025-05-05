#include "system_behavior_monitor_snippet.h"

#include "tools/reliability_mechanisms/single_design/error_detection/system_behavior_monitor.h"

behavior_monitor_t my_task_monitor;

void system_behavior_monitor_snippet() {

    behavior_monitor_init(&my_task_monitor, 1000000, "Task1"); // 1 секунда таймаут

    // Периодически сообщаем, что задача работает
    behavior_monitor_kick(&my_task_monitor);

    do_work();

    // Допустим, проверка состояния в Watchdog-таймере или супервайзере:
    behavior_monitor_check(&my_task_monitor);
}
