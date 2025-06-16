#include "checksum_validator.h"
#include "error_monitor/error_monitor.h"
#include <stdlib.h>

// --- SUM8 ---
uint8_t sum8(const uint8_t* data, uint32_t len) {
    uint8_t sum = 0;
    for (uint32_t i = 0; i < len; ++i) {
        sum += data[i];
    }
    return sum;
}

// --- SUM16 ---
uint16_t sum16(const uint8_t* data, uint32_t len) {
    uint16_t sum = 0;
    for (uint32_t i = 0; i < len; ++i) {
        sum += data[i];
    }
    return sum;
}

// --- CRC8 (полином 0x07) ---
uint8_t crc8(const uint8_t* data, uint32_t len) {
    uint8_t crc = 0x00;
    for (uint32_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; ++j) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
        }
    }
    return crc;
}

// --- CRC16 (полином 0x1021) ---
uint16_t crc16(const uint8_t* data, uint32_t len) {
    uint16_t crc = 0xFFFF;
    for (uint32_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; ++j) {
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : (crc << 1);
        }
    }
    return crc;
}

// --- CRC32 (полином 0xEDB88320) ---
uint32_t crc32(const uint8_t* data, uint32_t len) {
    uint32_t crc = 0xFFFFFFFF;
    for (uint32_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; ++j) {
            crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320 : (crc >> 1);
        }
    }
    return ~crc;
}

// Существующие SUM и CRC функции (как выше)

// Новый код Хэмминга (7,4):
// кодируем 4 бита данных в 7 бит с проверкой ошибок
uint8_t hamming74_encode(uint8_t nibble) {
    uint8_t d1 = (nibble >> 0) & 0x1;
    uint8_t d2 = (nibble >> 1) & 0x1;
    uint8_t d3 = (nibble >> 2) & 0x1;
    uint8_t d4 = (nibble >> 3) & 0x1;

    uint8_t p1 = d1 ^ d2 ^ d4;
    uint8_t p2 = d1 ^ d3 ^ d4;
    uint8_t p3 = d2 ^ d3 ^ d4;

    uint8_t encoded = (p1 << 6) | (p2 << 5) | (d1 << 4) | (p3 << 3) | (d2 << 2) | (d3 << 1) | d4;
    return encoded;
}

uint8_t hamming74_decode(uint8_t encoded, uint8_t* error_corrected, uint8_t* uncorrectable_error) {
    uint8_t p1 = (encoded >> 6) & 0x1;
    uint8_t p2 = (encoded >> 5) & 0x1;
    uint8_t d1 = (encoded >> 4) & 0x1;
    uint8_t p3 = (encoded >> 3) & 0x1;
    uint8_t d2 = (encoded >> 2) & 0x1;
    uint8_t d3 = (encoded >> 1) & 0x1;
    uint8_t d4 = (encoded >> 0) & 0x1;

    uint8_t s1 = p1 ^ d1 ^ d2 ^ d4;
    uint8_t s2 = p2 ^ d1 ^ d3 ^ d4;
    uint8_t s3 = p3 ^ d2 ^ d3 ^ d4;

    uint8_t syndrome = (s1 << 2) | (s2 << 1) | (s3 << 0);

    if (syndrome != 0) {
        if (syndrome <= 7) {
            encoded ^= (1 << (7 - syndrome)); // исправляем ошибочный бит
            *error_corrected = 1;
            *uncorrectable_error = 0;
        } else {
            *uncorrectable_error = 1;
            return 0xFF; // Невозможно исправить
        }
    } else {
        *error_corrected = 0;
        *uncorrectable_error = 0;
    }

    uint8_t decoded = ((encoded >> 4) & 0x1) | ((encoded >> 2) & 0x2) | ((encoded >> 1) & 0x4) | ((encoded >> 0) & 0x8);
    return decoded;
}

// Расширяем checksum_calculate для Хэмминга
uint32_t checksum_calculate(checksum_type_t type, const uint8_t* data, uint32_t length) {
    switch (type) {
        case CHECKSUM_SUM8:  return sum8(data, length);
        case CHECKSUM_SUM16: return sum16(data, length);
        case CHECKSUM_CRC8:  return crc8(data, length);
        case CHECKSUM_CRC16: return crc16(data, length);
        case CHECKSUM_CRC32: return crc32(data, length);
        case CHECKSUM_HAMMING_7_4:
        {
            uint32_t result = 0;
            for (uint32_t i = 0; i < length; i++) {
                result ^= hamming74_encode(data[i] & 0x0F); // только нижние 4 бита
            }
            return result;
        }
        default: return 0;
    }
}
