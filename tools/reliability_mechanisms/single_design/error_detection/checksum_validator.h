#ifndef CHECKSUM_VALIDATOR_H
#define CHECKSUM_VALIDATOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        CHECKSUM_SUM8,
        CHECKSUM_SUM16,
        CHECKSUM_CRC8,
        CHECKSUM_CRC16,
        CHECKSUM_CRC32,
        CHECKSUM_HAMMING_7_4   // Новый тип — код Хэмминга
    } checksum_type_t;

    /// Расчёт контрольной суммы
    uint32_t checksum_calculate(checksum_type_t type, const uint8_t* data, uint32_t length);

    /// Проверка контрольной суммы
    bool checksum_validate(checksum_type_t type, const uint8_t* data, uint32_t length, uint32_t expected, const char* context);

    /// Кодирование одного байта кодом Хэмминга (7,4)
    uint8_t hamming74_encode(uint8_t nibble);

    /// Декодирование одного байта кодом Хэмминга (7,4) с исправлением ошибки
    uint8_t hamming74_decode(uint8_t encoded, bool* error_corrected, bool* uncorrectable_error);

#ifdef __cplusplus
}
#endif

#endif // CHECKSUM_VALIDATOR_H
