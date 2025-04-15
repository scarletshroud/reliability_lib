#ifndef ERROR_MONITOR_H_
#define ERROR_MONITOR_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ERROR_LOG_CAPACITY 256

typedef enum {
    ERROR_LEVEL_INFO = 0,
    ERROR_LEVEL_WARNING,
    ERROR_LEVEL_ERROR,
    ERROR_LEVEL_CRITICAL
} error_level_t; 

typedef struct {
    uint32_t timestamp;
    uint32_t line;
    const char* file;
    const char* module;
    const char* message;
    error_level_t error_level;
} error_event_t;

typedef enum {
    ERROR_MONITOR_OP_OK = 0,
    ERROR_MONITOR_OVERFLOW
} error_monitor_op_status_t; 

/**
 * 
 */
error_monitor_op_status_t error_monitor_save_event(const char* file, const char* module, const char* message, uint32_t line, error_level_t error_level); 

/**
 * 
 */
void error_monitor_clear(void);

/**
 * 
 */
uint32_t error_monitor_err_cnt();

//const error_event_t* error_monitor_get_recent();

#ifdef __cplusplus
}
#endif

#endif