#ifndef N_VERSION_EXEC_H
#define N_VERSION_EXEC_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    /// Поддержка до 5 версий одной функции
#define MAX_N_VERSIONS 5

/// Интерфейс функции-версии
    typedef bool (*n_version_fn_t)(void* result, const void* input);
    typedef bool (*n_version_compare_fn_t)(const void* r1, const void* r2);
    typedef void (*n_version_copy_fn_t)(void* dst, const void* src);

    typedef enum {
        N_VERSION_VOTING_STRICT,
        N_VERSION_VOTING_MAJORITY,
        N_VERSION_VOTING_THRESHOLD  // новый!
    } n_version_voting_mode_t;

    typedef struct {
        n_version_fn_t versions[MAX_N_VERSIONS];
        uint32_t version_count;
        uint32_t threshold; // для threshold режима
        n_version_compare_fn_t compare;
        n_version_copy_fn_t copy;
        n_version_voting_mode_t mode;
        const char* context;
    } n_version_exec_t;

    /// Запуск всех версий и выбор результата по большинству
    bool n_version_execute(
        const n_version_exec_t* exec,
        const void* input,
        void** result_buffers,
        void* selected_output
    );

#ifdef __cplusplus
}
#endif

#endif // N_VERSION_EXEC_H
