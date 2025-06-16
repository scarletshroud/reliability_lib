#include "process_pair.h"

#include "tools/tools_config.h"
#include "error_monitor/error_monitor.h"

#ifndef TARGET_FREERTOS

bool process_pair_execute(
    const process_pair_t* pair,
    const void* input,
    void* result_main,
    void* result_backup
) {
    if (!pair || !pair->active || !pair->backup || !pair->compare) return false;

    bool main_ok = pair->active(result_main, input);

    if (pair->mode == PROCESS_PAIR_ACTIVE_PASSIVE) {
        if (!main_ok) {

#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, pair->context, "Active failed, switching to passive", __LINE__, ERROR_LEVEL_WARNING);
#endif

            bool backup_ok = pair->backup(result_backup, input);
            if (!backup_ok) {

#ifdef ERROR_MONITOR_ENABLE
                error_monitor_save_event(__FILE__, pair->context, "Passive also failed", __LINE__, ERROR_LEVEL_ERROR);
#endif

                return false;
            }
            return true;
        }
        return true;
    }

    if (pair->mode == PROCESS_PAIR_ACTIVE_ACTIVE) {
        bool backup_ok = pair->backup(result_backup, input);
        if (!main_ok || !backup_ok) {

#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, pair->context, "One of active processes failed", __LINE__, ERROR_LEVEL_ERROR);
#endif

            return false;
        }

        if (!pair->compare(result_main, result_backup)) {

#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, pair->context, "Mismatch between active-active results", __LINE__, ERROR_LEVEL_ERROR);
#endif

            return false;
        }
        return true;
    }

    return false;
}

#else

#include "process_pair.h"
#include "error_monitor/error_monitor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define PROCESS_TIMEOUT pdMS_TO_TICKS(100)

typedef struct {
    const void* input;
    void* result;
    bool success;
    SemaphoreHandle_t done;
    bool (*process_func)(void*, const void*);
} process_task_params_t;

static void process_task_runner(void* params_void) {
    process_task_params_t* params = (process_task_params_t*)params_void;
    params->success = params->process_func(params->result, params->input);
    xSemaphoreGive(params->done);
    vTaskDelete(NULL);
}

bool process_pair_execute(
    const process_pair_t* pair,
    const void* input,
    void* result_main,
    void* result_backup
) {
    if (!pair || !pair->active || !pair->backup || !pair->compare) return false;

    // Подготовка параметров задач
    SemaphoreHandle_t sem_main = xSemaphoreCreateBinary();
    SemaphoreHandle_t sem_backup = xSemaphoreCreateBinary();

    if (!sem_main || !sem_backup) return false;

    process_task_params_t main_params = {
        .input = input,
        .result = result_main,
        .process_func = pair->active,
        .done = sem_main,
        .success = false
    };

    process_task_params_t backup_params = {
        .input = input,
        .result = result_backup,
        .process_func = pair->backup,
        .done = sem_backup,
        .success = false
    };

    if (pair->mode == PROCESS_PAIR_ACTIVE_PASSIVE) {
        xTaskCreate(process_task_runner, "MainProc", 512, &main_params, tskIDLE_PRIORITY + 1, NULL);

        if (xSemaphoreTake(sem_main, PROCESS_TIMEOUT) == pdFALSE || !main_params.success) {
#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, pair->context, "Active failed, switching to passive", __LINE__, ERROR_LEVEL_WARNING);
#endif

            xTaskCreate(process_task_runner, "BackupProc", 512, &backup_params, tskIDLE_PRIORITY + 1, NULL);
            if (xSemaphoreTake(sem_backup, PROCESS_TIMEOUT) == pdFALSE || !backup_params.success) {

#ifdef ERROR_MONITOR_ENABLE
                error_monitor_save_event(__FILE__, pair->context, "Passive also failed", __LINE__, ERROR_LEVEL_ERROR);
#endif

                vSemaphoreDelete(sem_main);
                vSemaphoreDelete(sem_backup);
                return false;
            }
        }

        vSemaphoreDelete(sem_main);
        vSemaphoreDelete(sem_backup);
        return true;
    }

    if (pair->mode == PROCESS_PAIR_ACTIVE_ACTIVE) {
        xTaskCreate(process_task_runner, "MainProc", 512, &main_params, tskIDLE_PRIORITY + 1, NULL);
        xTaskCreate(process_task_runner, "BackupProc", 512, &backup_params, tskIDLE_PRIORITY + 1, NULL);

        bool main_done = (xSemaphoreTake(sem_main, PROCESS_TIMEOUT) == pdTRUE);
        bool backup_done = (xSemaphoreTake(sem_backup, PROCESS_TIMEOUT) == pdTRUE);

        if (!main_done || !backup_done || !main_params.success || !backup_params.success) {

#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, pair->context, "One of active processes failed", __LINE__, ERROR_LEVEL_ERROR);
#endif

            vSemaphoreDelete(sem_main);
            vSemaphoreDelete(sem_backup);
            return false;
        }

        if (!pair->compare(result_main, result_backup)) {

#ifdef ERROR_MONITOR_ENABLE
            error_monitor_save_event(__FILE__, pair->context, "Mismatch between active-active results", __LINE__, ERROR_LEVEL_ERROR);
#endif

            vSemaphoreDelete(sem_main);
            vSemaphoreDelete(sem_backup);
            return false;
        }

        vSemaphoreDelete(sem_main);
        vSemaphoreDelete(sem_backup);
        return true;
    }

    vSemaphoreDelete(sem_main);
    vSemaphoreDelete(sem_backup);
    return false;
}

#endif