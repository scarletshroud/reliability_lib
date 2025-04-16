#include "error_monitor.h"

static uint32_t error_idx = 0;
static uint32_t errors_cnt = 0;

static error_event_t error_monitor_buffer[ERROR_LOG_CAPACITY];

static uint32_t fake_timestamp = 0;

error_monitor_op_status_t error_monitor_save_event(const char* file, const char* module, const char* message, uint32_t line, error_level_t error_level) {

    if (error_idx >= ERROR_LOG_CAPACITY) {
        return ERROR_MONITOR_OVERFLOW;
    }

    error_event_t* e = &error_monitor_buffer[error_idx];

    /* Fill the structure */

    e->timestamp = fake_timestamp;
    e->line = line;
    e->module = module;
    e->file = file;
    e->module = module;
    e->message = message;
    e->error_level = error_level;

    error_idx++;
    errors_cnt++;

    return ERROR_MONITOR_OP_OK;
}

void error_monitor_clear(void) {
    error_idx = 0;
    errors_cnt = 0;

    fake_timestamp = 0;
}

uint32_t error_monitor_err_cnt(void) {
    return errors_cnt;
}
