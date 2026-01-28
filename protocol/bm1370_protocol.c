#include "bm1370_protocol.h"
#include <string.h>
#include <stdbool.h>
#include "crc16.h"
#include "crc5.h"

int bm1370_build_packet(
    uint8_t *buffer,
    size_t buffer_size,
    bm1370_packet_type_t type,
    const uint8_t *data,
    size_t data_len
) {
    size_t offset = 0;
    size_t total_length;

    // Validate input pointers
    if (!buffer || !data) {
        return -1;
    }

    // Compute total packet length based on packet type
    if (type == BM1370_PACKET_JOB) {
        total_length = data_len + 6;
    } else if (type == BM1370_PACKET_CMD) {
        total_length = data_len + 5;
    } else {
        return -1;
    }

    // Ensure output buffer is large enough
    if (buffer_size < total_length) {
        return -1;
    }

    // Add protocol preamble (packet synchronization bytes)
    buffer[offset++] = 0x55;
    buffer[offset++] = 0xAA;

    // Add header byte (0x20 = job, 0x40 = command)
    buffer[offset++] = (type == BM1370_PACKET_JOB) ? 0x20 : 0x40;

    // Add payload length field (protocol-defined)
    buffer[offset++] = (type == BM1370_PACKET_JOB)
        ? (uint8_t)(data_len + 4)
        : (uint8_t)(data_len + 3);

    // Copy payload data into packet
    memcpy(&buffer[offset], data, data_len);
    offset += data_len;

    // Append CRC (CRC16 for job, CRC5 for command)
    if (type == BM1370_PACKET_JOB) {
        uint16_t crc16_total = crc16_false(&buffer[2], data_len + 2);
        buffer[offset++] = (crc16_total >> 8) & 0xFF;
        buffer[offset++] = crc16_total & 0xFF;
    } else {
        buffer[offset++] = crc5_compute(buffer + 2, data_len + 2);
    }

    return (int)offset;
}
