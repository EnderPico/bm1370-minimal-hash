#include <stdint.h>
#include <string.h>
#include "crc.h"

// Minimal replica of ESP-Miner's _send_BM1370, without SERIAL_send
int esp_build_packet(
    uint8_t header,
    const uint8_t *data,
    uint8_t data_len,
    uint8_t *out_buf,
    size_t out_size
) {
    int is_job = header & 0x20;
    size_t total_len = is_job ? (data_len + 6) : (data_len + 5);

    if (out_size < total_len) return -1;

    out_buf[0] = 0x55;
    out_buf[1] = 0xAA;
    out_buf[2] = header;
    out_buf[3] = is_job ? (data_len + 4) : (data_len + 3);

    memcpy(out_buf + 4, data, data_len);

    if (is_job) {
        uint16_t crc = esp_crc16_false(out_buf + 2, data_len + 2);
        out_buf[4 + data_len] = (crc >> 8) & 0xFF;
        out_buf[5 + data_len] = crc & 0xFF;
    } else {
        out_buf[4 + data_len] = crc5(out_buf + 2, data_len + 2);
    }

    return total_len;
}
