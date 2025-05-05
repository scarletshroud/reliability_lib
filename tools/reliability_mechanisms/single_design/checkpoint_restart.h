#ifndef CHECKPOINT_RESTART_H
#define CHECKPOINT_RESTART_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        CHECKPOINT_MODE_STATIC,
        CHECKPOINT_MODE_DYNAMIC
    } checkpoint_mode_t;

    /// Интерфейс контрольной точки
    typedef struct {
        void* state_buffer;
        uint32_t size;
    } checkpoint_t;

    /// Статический перезапуск — вернуться к заранее сохранённой точке
    bool checkpoint_static_restart(checkpoint_t* checkpoint, void* current_state);

    /// Динамический перезапуск — сохранить и при необходимости восстановить
    bool checkpoint_dynamic_save(checkpoint_t* checkpoint, const void* state);
    bool checkpoint_dynamic_restore(const checkpoint_t* checkpoint, void* target);

#ifdef __cplusplus
}
#endif

#endif // CHECKPOINT_RESTART_H
