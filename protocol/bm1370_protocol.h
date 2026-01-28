#ifndef BM1370_PROTOCOL_H
#define BM1370_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

/*
 * BM1370 packet types
 */
typedef enum {
    BM1370_PACKET_CMD,
    BM1370_PACKET_JOB
} bm1370_packet_type_t;

/*
 * Packet structure:
 * [ PREAMBLE ][ HEADER ][ LENGTH ][ DATA... ][ CRC ]
 *
 * CMD packets use CRC5
 * JOB packets use CRC16
 */

/*
 * Build a BM1370 packet.
 *
 * buffer: output buffer
 * buffer_size: size of output buffer
 * type: command or job
 * data: payload
 * data_len: payload length
 *
 * returns: total packet length, or -1 on error
 */
int bm1370_build_packet(
    uint8_t *buffer,
    size_t buffer_size,
    bm1370_packet_type_t type,
    const uint8_t *data,
    size_t data_len
);

#endif
