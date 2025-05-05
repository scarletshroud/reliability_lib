#include "checksum_snippet.h"

#include "tools/reliability_mechanisms/single_design/error_detection/checksum_validator.h"

void checksum_snippet(void) {
    uint8_t buffer[] = { 1, 2, 3, 4, 5 };

    uint32_t crc = checksum_calculate(CHECKSUM_CRC16, buffer, sizeof(buffer));

    // Имитация ошибки
    buffer[2] ^= 0x01;

    if (!checksum_validate(CHECKSUM_CRC16, buffer, sizeof(buffer), crc, __func__)) {

    } else {

    }
}

void hamming_snippet(void) {
    uint8_t data = 0x05; // 4 бита данных

    uint8_t encoded = hamming74_encode(data);

    // Нарушаем один бит (симуляция ошибки)
    encoded ^= 0x10;

    bool corrected = false;
    bool uncorrectable = false;

    uint8_t decoded = hamming74_decode(encoded, &corrected, &uncorrectable);

    if (uncorrectable) {

    } else if (corrected) {

    } else {

    }
}
