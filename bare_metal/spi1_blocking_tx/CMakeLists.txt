cmake_minimum_required(VERSION 3.22)

project(spi1_blocking_tx C ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(TARGET_NAME spi1_blocking_tx)

add_executable(${TARGET_NAME}
        src/main.c
        src/system_stubs.c
        startup/startup_stm32g474xx.s
)

target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/Core/Include
        ${CMAKE_SOURCE_DIR}/vendor/Device/ST/STM32G4xx/Include
)

target_compile_definitions(${TARGET_NAME} PRIVATE
        STM32G474xx
)

target_compile_options(${TARGET_NAME} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -Wall
        -Wextra
        -Wpedantic

        -ffunction-sections
        -fdata-sections

        -O0
        -g3
)

target_link_options(${TARGET_NAME} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -T${CMAKE_SOURCE_DIR}/linker/STM32G474RETX_FLASH.ld

        -nostartfiles
        -specs=nano.specs
        -specs=nosys.specs

        -Wl,-Map=${CMAKE_BINARY_DIR}/${TARGET_NAME}.map
        -Wl,--gc-sections
)

add_custom_target(flash
        COMMAND /usr/bin/openocd
        -f interface/stlink.cfg
        -f target/stm32g4x.cfg
        -c "program $<TARGET_FILE:${TARGET_NAME}> verify reset exit"
        DEPENDS ${TARGET_NAME}
        COMMENT "Flashing ${TARGET_NAME} to STM32G474RE"
)