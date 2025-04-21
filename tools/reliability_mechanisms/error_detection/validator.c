#include "validator.h"

static validator_rule_t validator_rules[MAX_VALIDATOR_RULES];
static uint8_t rules_cnt = 0;

void validator_init(void) {
    rules_cnt = 0;
}

uint8_t validator_validate_num(uint8_t rule_id, double value) {

}

uint8_t validator_validate_buf(uint8_t rule_id, const uint8_t* buf, uint32_t len) {

}

