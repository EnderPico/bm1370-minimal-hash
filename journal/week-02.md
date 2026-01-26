Note:
This analysis is based on studying the open-source ESP-Miner project
(https://github.com/bitaxeorg/ESP-Miner).
All code remains property of its respective authors.
This document reflects my own understanding of the BM1370 control flow
derived from reading and tracing the source.

## ESP-Miner Mapping

# `_send_BM1370` Sequence:
1. Defines the packet type (`packet_type`) as a job packet (`JOB_PACKET`) or a command packet (`CMD_PACKET`)
  If job packet:
    1. Set `total_length` to `data_len` + 6
    2. Create a buffer with length `total_length`
    3. Add the preamble
    4. Add the header field
    5. Add the length field (`data_len` + 3)
    6. Add the data
    7. Add the correct crc type (error‑detection checksum, CRC16 for job packets)
  If command packet:
    1. Set `total_length` to `data_len` + 5
    2. Create a buffer with length `total_length`
    3. Add the preamble
    4. Add the header field
    5. Add the length field (`data_len` + 4)
    6. Add the data
    7. Add the correct crc type (error‑detection checksum, CRC5 for command packets)
2. Send the buffer using the `SERIAL_send` function

# Init Sequence:
1. At:main/main.c -> `asic_initialize` in line 102 
2. At:main/power/asic_init.c -> `ASIC_init` in line 46
3. At:components/asic/asic.c -> `BM1370_init` in line 30
4. At:components/asic/bm1370.c -> `_send_BM1370` in line 169

# Create Jobs Task Sequence
1. At:main/main.c -> `create_jobs_task` in lines 109
2. At:main/tasks/create_jobs_task.c -> `generate_work` in line 79
3. At:main/tasks/create_jobs_task.c -> `ASIC_send_work` in line 125
4. At:components/asic/asic.c -> `BM1370_send_work` in line 78
5. At:components/asic/bm1370.c -> `_send_BM1370` in line 331

# ASIC Result Task Sequence
1. main/main.c -> ASIC_result_task
2. main/tasks/asic_result_task.c -> ASIC_process_work
3. components/asic/asic.c -> BM1370_process_work
4. components/asic/bm1370.c -> receive_work()
5. components/asic/bm1370.c -> parse bm1370_asic_result_t
6. components/asic/bm1370.c -> fill task_result struct
7. Return task_result* to caller

# ASIC Response Handling

BM1370 responses are received as raw serial packets and parsed
inside `BM1370_process_work`.

Two response types exist:
- Register read responses
- Job (nonce found) responses

The parsed data is converted into a `task_result` struct, which
serves as the abstraction boundary between ASIC-specific logic
and the rest of the miner.
