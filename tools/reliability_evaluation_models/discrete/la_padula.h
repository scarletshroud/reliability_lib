#ifndef LA_PADULA_H_
#define LA_PADULA_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LA_PADULA_MAX_STEPS 256

    typedef struct {
        uint8_t step;
        uint32_t error_count[LA_PADULA_MAX_STEPS];
    } la_padula_model_data_t;

    void la_padula_model_reset();
    void la_padula_model_add_step();

    void la_padula();

#ifdef __cplusplus
}
#endif

#endif