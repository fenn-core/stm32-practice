# BME280 Readout

STM32 project reading temperature, pressure, and humidity data from a 
BME280 sensor and displaying the results over SSD1306 OLED and UART.

## Features
- BME280 I2C driver
- SSD1306 SPI OLED display output 
- framebuffer-based graphics/text rendering
- UART serial output

## Hardware
- STM32 Nucleo F446RE
- BME280 module
- SSD1306 128x64 SPI OLED

## Notes
- BME280 I2C address: 0x76
- BME280 chip ID: 0x60
- OLED uses SPI with GPIO CS/DC/RST 