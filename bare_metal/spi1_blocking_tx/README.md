CMSIS and STM32G4 device headers copied from STM32CubeG4
These files are vendor-owned and should not be edited directly
Local project code lives in src/

Simple, bare-metal, blocking, 8-bit spi data transmit experiment.
Bytes 0xAA, 0x55, 0xF0, 0x0F are transmitted over spi with:
PA5 acting as SCK and, 
PA7 acting as MOSI
 
Uses vendor provided startup, linker, and CMSIS headers.
