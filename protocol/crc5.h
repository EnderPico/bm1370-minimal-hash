#ifndef CRC5_H
#define CRC5_H

#include <stdint.h>
#include <stddef.h>

uint8_t crc5_compute(const uint8_t *data, size_t bit_len);

#endif
