#include "checkpoint_restart.h"

#include <string.h>

#include "error_monitor/error_monitor.h"

bool checkpoint_static_restart(checkpoint_t* checkpoint, void* current_state) {
    if (!checkpoint || !checkpoint->state_buffer || !current_state || checkpoint->size == 0) {
        return false;
    }

    memcpy(current_state, checkpoint->state_buffer, checkpoint->size);
    error_monitor_save_event(__FILE__, "Static Restart", "Restored static checkpoint", __LINE__, ERROR_LEVEL_WARNING);
    return true;
}

bool checkpoint_dynamic_save(checkpoint_t* checkpoint, const void* state) {
    if (!checkpoint || !checkpoint->state_buffer || !state || checkpoint->size == 0) {
        return false;
    }

    memcpy(checkpoint->state_buffer, state, checkpoint->size);
    return true;
}

bool checkpoint_dynamic_restore(const checkpoint_t* checkpoint, void* target) {
    if (!checkpoint || !checkpoint->state_buffer || !target || checkpoint->size == 0) {
        return false;
    }

    memcpy(target, checkpoint->state_buffer, checkpoint->size);
    error_monitor_save_event(__FILE__, "Dynamic Restart", "Restored dynamic checkpoint", __LINE__, ERROR_LEVEL_WARNING);
    return true;
}


/*

typedef struct {
    int position;
    int velocity;
    uint8_t error_count;
} SystemState;

SystemState current_state = {0};
uint8_t static_buffer[sizeof(SystemState)];
uint8_t dynamic_buffer[sizeof(SystemState)];

checkpoint_t static_cp = {
    .state_buffer = static_buffer,
    .size = sizeof(SystemState)
};

checkpoint_t dynamic_cp = {
    .state_buffer = dynamic_buffer,
    .size = sizeof(SystemState)
};

void example_restart_mechanism() {
    // Статическая инициализация
    memcpy(static_buffer, &current_state, sizeof(SystemState));

    // Работаем
    current_state.position = 100;
    current_state.velocity = 3;

    // Сохраняем динамическую точку
    checkpoint_dynamic_save(&dynamic_cp, &current_state);

    // Имитация сбоя
    current_state.error_count = 255;

    // Восстановление
    if (!checkpoint_dynamic_restore(&dynamic_cp, &current_state)) {
        checkpoint_static_restart(&static_cp, &current_state);
    }

    printf("Restored position: %d\n", current_state.position);
}

*/