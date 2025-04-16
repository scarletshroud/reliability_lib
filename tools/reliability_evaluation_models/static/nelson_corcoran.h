#ifndef NELSON_CORCORAN_H
#define NELSON_CORCORAN_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

    float nelson_corcoran_reability(float* reability, uint32_t all_runs, uint32_t failed_runs);

#ifdef __cplusplus
}
#endif

#endif 