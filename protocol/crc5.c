#include "crc5.h"

/*
 * CRC5 for BM1370 command packets
 * Bit-exact match to ESP-Miner
 */
uint8_t crc5_compute(const uint8_t *data, size_t len)
{
    uint8_t crc = 0x1F;

    for (size_t byte = 0; byte < len; byte++) {
        uint8_t current = data[byte];

        for (int bit = 0; bit < 8; bit++) {
            uint8_t input_bit = (current >> 7) & 1;
            current <<= 1;

            uint8_t new_bit = ((crc >> 4) ^ input_bit) & 1;
            crc = ((crc << 1) | new_bit) ^ (new_bit << 2);
            crc &= 0x1F;
        }
    }

    return crc;
}
