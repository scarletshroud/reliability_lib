#ifndef SHUMAN_BASED_H_
#define SHUMAN_BASED_H_

#include <stdint.h>

#define SHUMAN_BASED_MAX_TESTS_COUNT 256

/**
*
*/
typedef struct {
    uint8_t tests_cnt;          /*!< Point X coordinate */
    uint8_t req_intensity;      /*!< Интенсивность поступления заявок */
    uint8_t specified_time;     /*!< Заданное время t */
    uint8_t test_duration[SHUMAN_BASED_MAX_TESTS_COUNT];
    uint8_t test_errors_cnt[SHUMAN_BASED_MAX_TESTS_COUNT];
    float p_fault_transform;    /*!< Вероятность трансформации проявленной ошибки */
    float sys_fault_intensity;  /*!< Интенсивность отказов средств обеспечения отказоустойчивости системы */
    float p_fault_detect;       /*!< Вероятность правильного обнаружения отказов */
    float p_fault_process;      /*!< Вероятность успешного парирования обнаруженного отказа  */
    float t_sys_recovery;       /*!< Среднее время простоя информационной системы вследствие устранения ошибки программы */
} shuman_based_model_t;

/*
*
*/
typedef struct {
    uint32_t defects_cnt;   /*!< Первоначальное количество дефектов в программе */
    float p_error_free;     /*!< Вероятность отсутствия ошибки в результатах выполнения программы */
    float p_correct_exec;   /*!< Вероятность правильного одногкратного выполнения программы */
    float fault_intensity;  /*!< Интенсивность ошибок программы по Шуману */
    float mtbf;             /*!< Среднее время до ошибки программы */
    float p_fault_free;     /*!< Вероятность отсутствия отказов в результатах выполнения программы */
    float mtbf_t;           /*!< Среднее время до частичного функционального отказа */
    float k_ready;          /*!< Коэффициент частичной функциональной готовности */
} shuman_based_model_stats_t;

uint8_t shuman_collect_reability_stats(const shuman_based_model_t* model, shuman_based_model_stats_t* stats);

#endif