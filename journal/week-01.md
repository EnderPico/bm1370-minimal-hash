Week 01 – Phase 0
Focus: Understanding BM1370 basics and minimum requirements.

## Power Requirements
- The BM1370 operates with a low-voltage, high-current core supply and additional low-current IO rails. Reported total power consumption varies by clock configuration and workload; community sources typically cite values between ~17 W (nominal) and >20 W (worst-case). For bring-up and safety, I will assume worst-case conditions.
  - Input Power:
    - Typically supplied via a 5V rail
    - Must be capable of delivering sufficient power for >20 W total consumption (e.g., >4 A at 5 V, allowing margin for conversion losses)
  - Core Supply:
    - VDD (VDD1_0 through VDD3_1)
    - Nominal voltage: ~1.2 V
    - High current rail; peak current can reach approximately 15–20 A depending on operating conditions
    - In known reference designs (e.g., Bitaxe), this rail is generated using a high-current buck converter such as the TPS546D24
  - IO Supplies:
    - VDDIO_12 (_0, _1)
      - Nominal voltage: ~1.2 V
      - Low current rail, likely supplying IO logic and internal support circuitry
    - VDDIO_08 (_0, _1)
      - Nominal voltage: ~0.8 V
      - Low current rail; exact current requirements are not publicly documented, but are assumed to be significantly lower than the core supply
  - Ground:
    - VSS pins provide the common ground reference and must be connected with low-impedance paths to support high transient currents on the core rail

## Communication Interface
- The BM1370 uses a proprietary protocol that runs over UART-like structure. It uses bidirectional communication over 6 signals to send and receive data.
    - 4 signals from Esp32 to BM1370: CI, BI, NRST, PIN_MODE
    - 2 signals from BM1370 to Esp32: CO, BO


## Clocking
- It uses an external clock source with 25MHz that is connected to the BM1370's CLKI and CLKO pins.
    - CLKI: External clock input
    - CLKO: External clock output

## Known Minimal Setups
- The only minimal setup I could find is the one from Bitaxe but without the display as it is possible to access the interface thru the network.

## Open Questions
- While a lot of information exists, it’s scattered across different
sources and sometimes hard to piece together. Because of that, a few
things are still unclear:
  - I still need to understand the pin layout better, especially how the different power and IO domains are organized.
  - The communication protocol needs deeper study using available community resources (https://github.com/bitaxeorg/ESP-Miner — my friend, and likely yours too if you dig into this).
  - So far, I haven’t found any meaningful difference between the PA and BC variants beyond their cooling assumptions (water vs air).
  - The current requirements for VDDIO_08 (`_0` or `_1`) at 0.8 V are still unclear and don’t seem to be explicitly documented.
