#include "checksum_snippet.h"

#include "tools/reliability_mechanisms/single_design/error_detection/input_validation.h"

// void example_input_validation() {
//     int speed = 120;
//     const char* device_name = "Sensor1";

//     range_context_t speed_range = { 0, 150 };
//     strlen_context_t name_limit = { 10 };

//     validator_t validators[] = {
//         { "Speed", validate_range, &speed_range },
//         { "Device Name", validate_string_length, &name_limit }
//     };

//     const void* values[] = {
//         &speed,
//         device_name
//     };

//     bool all_valid = input_validate_all(validators, 2, values);

//     if (!all_valid) {
//         printf("Validation failed!\n");
//     } else {
//         printf("All inputs are valid.\n");
//     }
// }
