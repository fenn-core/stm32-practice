CMSIS and STM32G4 device headers copied from STM32CubeG4
These files are vendor-owned and should not be edited directly
Local project code lives in src/

Simple, bare-metal, non-blocking, onboard LED state machine.
The blue user button is used to switch between 4 modes:
permanently on,
blink period 500 ms
blink period 100 ms
permanently off 
 
Timing is implemented via Cortex-M core SysTick. 
Uses vendor provided startup, linker, and CMSIS headers.
