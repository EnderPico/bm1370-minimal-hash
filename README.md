BM1370 Minimal Hash Project

Goal:
Bring up a BM1370 Bitcoin ASIC with a minimal hardware and firmware
setup and demonstrate valid hash computation at low power.

Non-goals:
- Full 1.2 TH/s performance
- Profit-oriented mining
- Multi-chip scaling

Current status:
Phase 0 – research and architecture definition.

This project focuses on understanding, documentation, and
hardware/software co-design.

Controller:
Raspberry Pi Pico 2

Rationale:
Chosen for deterministic timing, strong debugging support,
and flexibility via PIO for custom protocols.
