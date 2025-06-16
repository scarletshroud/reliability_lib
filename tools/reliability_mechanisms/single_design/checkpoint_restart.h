#ifndef CHECKPOINT_RESTART_H
#define CHECKPOINT_RESTART_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * \brief           Перечисление с возможными типами работы механизма контрольных точек
    */
    typedef enum {
        CHECKPOINT_MODE_STATIC,     /*!< Статические контрольные точки */
        CHECKPOINT_MODE_DYNAMIC     /*!< Динамические контрольные точки */
    } checkpoint_mode_t;

    /**
     * \brief           Структура контрольной точки
    */
    typedef struct {
        void* state_buffer;     /*!< Данные о состоянии */
        uint32_t size;          /*!< Размер данных о состоянии */
    } checkpoint_t;

    /**
     * \brief           Функция возврата к статической контрольной точке
     * \param[in]       checkpoint: Сохраненная контрольная точка
     * \param[in]       current_state: Текущее состояние
     * \return          Успешность операции: 0 - ошибка, 1 - успешно
    */
    uint8_t checkpoint_static_restart(checkpoint_t* checkpoint, void* current_state);

    /**
     * \brief           Функция сохранения динамической контрольной точки
     * \param[in]       checkpoint: Контрольная точка
     * \param[in]       state: Текущее состояние
     * \return          Успешность операции: 0 - ошибка, 1 - успешно
    */
    uint8_t checkpoint_dynamic_save(checkpoint_t* checkpoint, const void* state);

    /**
     * \brief           Функция возврата к динамической контрольной точке
     * \param[in]       checkpoint: Сохраненная контрольная точка
     * \param[in]       target: Текущее состояние
     * \return          Успешность операции: 0 - ошибка, 1 - успешно
    */
    uint8_t checkpoint_dynamic_restore(const checkpoint_t* checkpoint, void* target);

#ifdef __cplusplus
}
#endif

#endif // CHECKPOINT_RESTART_H
