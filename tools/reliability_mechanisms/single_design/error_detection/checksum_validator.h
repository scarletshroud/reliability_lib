#ifndef CHECKSUM_VALIDATOR_H
#define CHECKSUM_VALIDATOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Перечисление поддерживаемых типов алгоритмов контрольных сумм.
    */
    typedef enum {
        CHECKSUM_SUM8,          /*!< Простая 8-битная сумма */
        CHECKSUM_SUM16,         /*!< Простая 16-битная сумма */
        CHECKSUM_CRC8,          /*!< CRC-8 (циклический избыточный код) */
        CHECKSUM_CRC16,         /*!< crc-16 */
        CHECKSUM_CRC32,         /*!< crc-32 */
        CHECKSUM_HAMMING_7_4    /*!< Хэмминг (7,4) */
    } checksum_type_t;

    /**
     * @brief Вычисляет контрольную сумму для заданного блока данных.
     *
     * @param[in] type Тип алгоритма, используемого для расчёта.
     * @param[in] data Указатель на буфер входных данных.
     * @param[in] length Длина буфера в байтах.
     * @return Вычисленная контрольная сумма (тип зависит от выбранного алгоритма).
    */
    uint32_t checksum_calculate(checksum_type_t type, const uint8_t* data, uint32_t length);

    /**
     * @brief Проверяет контрольную сумму блока данных.
     *
     * @param[in] type Тип алгоритма, используемого для проверки.
     * @param[in] data Указатель на буфер входных данных.
     * @param[in] length Длина буфера в байтах.
     * @param[in] expected Ожидаемое значение контрольной суммы.
     * @param[in] context Строка контекста (например, имя источника данных) для отладки и логирования.
     * @return true, если контрольная сумма совпадает с ожидаемой; false в противном случае.
    */
    uint8_t checksum_validate(checksum_type_t type, const uint8_t* data, uint32_t length, uint32_t expected, const char* context);
    
    /**
     * @brief Кодирует 4-битный полубайт (nibble) по схеме Хэмминга (7,4).
     *
     * @param[in] nibble 4-битное значение (младшие 4 бита будут закодированы).
     * @return 7-битный код Хэмминга, упакованный в младшие 7 бит возвращаемого байта.
    */
    uint8_t hamming74_encode(uint8_t nibble);

    /**
     * @brief Декодирует 7-битное значение, закодированное по схеме Хэмминга (7,4).
     *
     * @param[in] encoded Закодированное значение.
     * @param[out] error_corrected Указатель на флаг, устанавливаемый в true, если была исправлена одиночная ошибка.
     * @param[out] uncorrectable_error Указатель на флаг, устанавливаемый в true, если ошибка не может быть исправлена.
     * @return Декодированное 4-битное значение (младшие 4 бита).
    */
    uint8_t hamming74_decode(uint8_t encoded, uint8_t* error_corrected, uint8_t* uncorrectable_error);

#ifdef __cplusplus
}
#endif

#endif // CHECKSUM_VALIDATOR_H
