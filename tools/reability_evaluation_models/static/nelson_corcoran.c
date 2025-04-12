#include "nelson_corcoran.h"

#include <stdint.h>

uint8_t nelson_corcoran_reability(float* reability, uint32_t all_runs, uint32_t failed_runs) {
    uint8_t status = 1;
    *reability = 0.0f;

    if (all_runs >= failed_runs) {
        *reability = 1 - (failed_runs / all_runs);
        status = 0;
    }

    return status;
}