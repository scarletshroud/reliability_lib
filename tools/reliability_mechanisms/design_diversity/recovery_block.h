#ifndef RECOVERY_BLOCK_H
#define RECOVERY_BLOCK_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_RB_VARIANTS 4

    typedef bool (*rb_variant_fn_t)(void* result, const void* input);
    typedef bool (*rb_acceptance_test_fn_t)(const void* result);
    typedef bool (*rb_compare_fn_t)(const void* result1, const void* result2);
    typedef void (*rb_copy_fn_t)(void* dest, const void* src);

    typedef enum {
        RB_MODE_SEQUENTIAL, // обычный Recovery Block
        RB_MODE_CONSENSUS   // CRB (все варианты выполняются, сравниваются)
    } rb_mode_t;


    typedef struct {
        rb_variant_fn_t variants[MAX_RB_VARIANTS];
        uint32_t variant_count;
        rb_acceptance_test_fn_t acceptance_test;     // для SEQUENTIAL
        rb_compare_fn_t consensus_compare;           // для CONSENSUS
        rb_copy_fn_t consensus_copy;                 // для CONSENSUS
        rb_mode_t mode;
        const char* context;
    } recovery_block_t;


    /// Выполнить восстановительный блок
    bool recovery_block_execute(
        const recovery_block_t* rb,
        const void* input,
        void* result
    );

#ifdef __cplusplus
}
#endif

#endif // RECOVERY_BLOCK_H
