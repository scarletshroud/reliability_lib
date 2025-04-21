#ifndef VALIDATOR_H_
#define VALIDATOR_H_

#include <stdint.h>

#define MAX_VALIDATOR_RULES 10

typedef enum {
    VALIDATE_LEVEL_DEBUG, // (it's the default, the usual assert)
    VALIDATE_LEVEL_WARNING, // (it's just a warning, the program can continue without user intervention)
    VALIDATE_LEVEL_ERROR, // an error
} validate_level_t;

typedef bool (*validator_func)(const void* value, void* context);

typedef struct {
    uint8_t id;
    const char* name;
    validator_func func;
    validate_level_t validate_level;
    void* context;
} validator_rule_t;

/**
 *  Инициализация модуля (вызывается на старте)
 */
void validator_init(void);

/**
 *  Проверить число (float/int)
 *  @rule_id  — идентификатор правила из rules[]
 *  @value    — сам параметр
 */
uint8_t validator_validate_num(uint8_t rule_id, double value);

/**
 *  Проверить буфер данных (например, пакет):
 *  @rule_id  — идентификатор правила
 *  @buf      — указатель на данные
 *  @len      — длина данных
 */
uint8_t validator_validate_buf(uint8_t rule_id, const uint8_t* buf, uint32_t len);

#endif
