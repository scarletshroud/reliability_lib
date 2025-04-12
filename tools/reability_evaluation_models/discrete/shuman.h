#ifndef SHUMAN_H_
#define SHUMAN_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SHUMAN_MAX_STEPS 256

    typedef struct {
        uint32_t total_tests;
        uint32_t steps_count;
        uint32_t erros_count[SHUMAN_MAX_STEPS];
    } shuman_model_data_t;

    void shuman_model_reset();
    void shuman_model_add_step();
    void shuman_calc(shuman_model_data_t* shuman_data);

#ifdef __cplusplus
}
#endif

#endif