# MPU6500 ADI

STM32 project that collects attitude data from an MPU6500 sensor
and plots a minimal line ADI on an SSD1306 display.

## Features
- MPU6500 I2C driver
- SSD1306 SPI OLED display output 
- framebuffer-based graphics/text rendering

## Hardware
- STM32 Nucleo F446RE
- MPU6500 module
- SSD1306 128x64 SPI OLED

## Pinout

### MPU6500 (I2C) 

| Pin | Board | Description     |
|-----|-------|-----------------|
| GND |  GND  | Ground          |
| VCC |  3V3  | 3.3 V           |
| SCL |  PB6  |  Clock          |
| SDA |  PB7  |  Data           |

---

### SSD1306 (SPI)

| Pin | Board | Description |
|-----|-------|-------------|
| GND |  GND  |   Ground    |
| VCC |  3V3  |   3.3 V     |
| SCK |  PB10 |  SPI Clock  |
| SDA |  PC1  |  SPI Data   |
| RES |  PA8  |    Reset    |
| DC  |  PA10 | Data/Cmd Select|
| CS  |  PA9  | Chip Select |



## Notes
- MPU6500 I2C address: 0x68
- MPU6500 chip ID: 0x70
- OLED uses SPI with GPIO CS/DC/RST 