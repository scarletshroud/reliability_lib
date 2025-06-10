#include <stdint.h>

#include "snippets/assert_snippet.h"
#include "tools/error_monitor/error_monitor.h"
#include "tools/reliability_evaluation_models/discrete/shuman_based.h"

#include "platform/adapter/platform_adapter.h"
#include "platform/mk32_baremetal/src/mk32_system.h"


int main() {
    /* System init */
    platform_init();
    platform_print("System was successfully initialized.\n");

    

    return 0;
}
