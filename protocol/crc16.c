#include "crc16.h"

/*
 * CRC16-FALSE
 * Polynomial: 0x1021
 * Init: 0xFFFF
 * No reflection
 * No final XOR
 */
uint16_t crc16_false(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xFFFF;

    while (len--) {
        crc ^= (uint16_t)(*data++) << 8;

        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}
