#ifndef PROCESS_PAIR_H
#define PROCESS_PAIR_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        PROCESS_PAIR_ACTIVE_PASSIVE,
        PROCESS_PAIR_ACTIVE_ACTIVE
    } process_pair_mode_t;

    typedef bool (*process_fn_t)(void* result, const void* input);
    typedef bool (*process_compare_fn_t)(const void* result1, const void* result2);

    typedef struct {
        process_fn_t active;
        process_fn_t backup;
        process_compare_fn_t compare;
        process_pair_mode_t mode;
        const char* context;
    } process_pair_t;

    /// Выполнить парную обработку в заданной схеме
    bool process_pair_execute(
        const process_pair_t* pair,
        const void* input,
        void* result_main,
        void* result_backup
    );

#ifdef __cplusplus
}
#endif

#endif // PROCESS_PAIR_H