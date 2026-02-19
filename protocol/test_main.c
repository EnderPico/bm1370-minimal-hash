#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "bm1370_protocol.h"

// Reference builder from ESP-Miner (test adapter)
int esp_build_packet(
    uint8_t header,
    const uint8_t *data,
    uint8_t data_len,
    uint8_t *out_buf,
    size_t out_size
);

static void dump(const char *label, const uint8_t *buf, int len)
{
    printf("%s (%d bytes): ", label, len);
    for (int i = 0; i < len; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

static void compare_packets(
    const char *name,
    const uint8_t *mine,
    int len_mine,
    const uint8_t *esp,
    int len_esp
)
{
    dump("Mine", mine, len_mine);
    dump("ESP ", esp,  len_esp);

    if (len_mine != len_esp || memcmp(mine, esp, len_mine) != 0) {
        printf("❌ %s PACKETS DIFFER\n\n", name);
    } else {
        printf("✅ %s PACKETS MATCH\n\n", name);
    }
}

int main(void)
{
    /* =======================
     * CMD PACKET TEST
     * ======================= */
    uint8_t cmd_payload[] = { 0x01, 0x02, 0x03 };

    uint8_t mine_cmd[64];
    uint8_t esp_cmd[64];

    int len_mine_cmd = bm1370_build_packet(
        mine_cmd,
        sizeof(mine_cmd),
        BM1370_PACKET_CMD,
        cmd_payload,
        sizeof(cmd_payload)
    );

    int len_esp_cmd = esp_build_packet(
        0x40, // TYPE_CMD
        cmd_payload,
        sizeof(cmd_payload),
        esp_cmd,
        sizeof(esp_cmd)
    );

    printf("=== CMD PACKET TEST ===\n");
    compare_packets("CMD", mine_cmd, len_mine_cmd, esp_cmd, len_esp_cmd);

    /* =======================
     * JOB PACKET TEST
     * ======================= */
    uint8_t job_payload[] = {
        // Fake job data – contents don’t matter, only framing & CRC
        0xDE, 0xAD, 0xBE, 0xEF,
        0x01, 0x23, 0x45, 0x67,
        0x89, 0xAB, 0xCD, 0xEF
    };

    uint8_t mine_job[128];
    uint8_t esp_job[128];

    int len_mine_job = bm1370_build_packet(
        mine_job,
        sizeof(mine_job),
        BM1370_PACKET_JOB,
        job_payload,
        sizeof(job_payload)
    );

    int len_esp_job = esp_build_packet(
        0x20, // TYPE_JOB
        job_payload,
        sizeof(job_payload),
        esp_job,
        sizeof(esp_job)
    );

    printf("=== JOB PACKET TEST ===\n");
    compare_packets("JOB", mine_job, len_mine_job, esp_job, len_esp_job);

    return 0;
}
