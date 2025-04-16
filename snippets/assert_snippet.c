#include "assert_snippet.h"

#include <stdlib.h>

#include "error_monitor/error_monitor.h"

#define RAND_MAX_CASE 5

void no_assert_snippet(void) {
    uint8_t system_error_case = 0;
    system_error_case = rand() % RAND_MAX_CASE;

    if (system_error_case == 0) {
        error_monitor_save_event(__FILE__, "no_assert_snippet", "error", __LINE__, ERROR_LEVEL_ERROR);
    } 
}

void assert_snippet(void) {
    uint8_t system_error_case = 0;
    system_error_case = rand() % RAND_MAX_CASE;

    if (system_error_case == 0) {

    }
    
    error_monitor_save_event(__FILE__, "no_assert_snippet", "error", __LINE__, ERROR_LEVEL_ERROR);
}