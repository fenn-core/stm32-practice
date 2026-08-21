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

## Pinout

### BME280 (I2C) 

| Pin | Board | Description            |
|-----|-------|------------------------|
| GND |  GND  | Ground                 |
| VCC |  3V3  | 3.3 V                  |
| SCL |  D10  |  Clock          |
| SDA |  PB7  |  Data           |

---

### SSD1306 (SPI)

| Pin | Board | Description |
|-----|-------|-------------|
| GND |  GND  |   Ground    |
| VCC |  3V3  |   3.3 V     |
| SCK |  D6   |  SPI Clock  |
| SDA |  PC1  |  SPI Data   |
| RES |  PC0  |    Reset    |
| DC  |  PC3  | Data/Cmd Select|
| CS  |  PA0  | Chip Select     |



## Notes
- BME280 I2C address: 0x76
- BME280 chip ID: 0x60
- OLED uses SPI with GPIO CS/DC/RST 