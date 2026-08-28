# STM32F411RE Bare-Metal Peripheral Drivers

This repository contains bare-metal peripheral drivers for the STM32F411RE (ARM Cortex-M4), written directly against CMSIS — no HAL or LL libraries. Each project targets a specific peripheral or concept, built up incrementally from basic GPIO/debugging to DMA-driven communication and ADC/PWM control.

The goal of this repo is to understand and implement peripheral behavior at the register level, without relying on ST's abstraction layers.

## Hardware

- **MCU:** STM32F411RE
- **Board:** Nucleo-F411RE <!-- update if different -->
- **IDE:** STM32CubeIDE
- **Libraries:** CMSIS only (no HAL/LL)

## Projects

| # | Project | Description |
|---|---------|-------------|
| 0 | `0_test` | Initial project setup / sanity check |
| 1 | `1_DebuggingBasic` | Basic debugging setup and workflow in CubeIDE |
| 2 | `2_WorkingWithITM` | Using ITM (Instrumentation Trace Macrocell) for real-time debug output |
| 3 | `3_WorkingWithTM-Printf` | Redirecting `printf` output through ITM/trace |
| 4 | `4_WorkingWithITM-Printf_logs` | Structured logging using ITM/printf |
| 5 | `5_WorkingWithUART` | Basic UART transmit/receive, register-level configuration |
| 6 | `6_DebuggingGPIO` | GPIO configuration and debugging fundamentals |
| 7 | `7_dma_mem_to_mem` | Memory-to-memory data transfer using DMA |
| 8 | `8_adc_dma` | ADC sampling with DMA-based data transfer |
| 9 | `9_adc_dma_timer_trigger` | Timer-triggered ADC conversions streamed via DMA |
| 10 | `10_uart_dma` | UART communication using DMA for non-blocking transfers |
| 11 | `11_spi_dma` | SPI communication using DMA |
| 12 | `12_i2c_dma` | I2C communication using DMA |
| 13 | `13_pwm_driver` | PWM signal generation using timers |
| 14 | `14_pwm_dma_driver` | PWM generation with DMA-driven duty cycle updates |

> This table will be updated as new projects are added.

## Demos

Video demonstrations for each project will be added here over time.

| Project | Video |
|---------|-------|
| _coming soon_ | |

## Repository Structure
├── 0_test/
├── 1_DebuggingBasic/
├── ...
├── 14_pwm_dma_driver/
└── README.md

Each project folder is a standalone STM32CubeIDE project containing its own `Inc`, `Src`, and startup files.

## Building a Project

1. Open STM32CubeIDE.
2. Import the desired project folder (`File → Open Projects from File System`).
3. Build and flash to a Nucleo-F411RE board via ST-Link.

## Notes

- All drivers are written directly against CMSIS device headers (register-level access), without HAL or LL.
- This is an active learning repository — projects and documentation will continue to be added and refined.
