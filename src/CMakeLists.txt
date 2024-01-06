cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Pull in FreeRTOS
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(main_blinky_smp
        main.c
        main_blinky.c
)

target_compile_definitions(main_blinky_smp PRIVATE
        mainCREATE_SIMPLE_BLINKY_DEMO_ONLY=1
)

target_compile_options(main_blinky_smp PUBLIC
        ### Gnu/Clang C Options
        $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
        #$<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>
)

target_include_directories(main_blinky_smp PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/Common/include)

target_link_libraries(main_blinky_smp pico_stdlib hardware_i2c
        FreeRTOS-Kernel FreeRTOS-Kernel-Heap1 pico_cyw43_arch_none)
pico_add_extra_outputs(main_blinky_smp)

# Use USB uart
pico_enable_stdio_usb(main_blinky_smp 1)
pico_enable_stdio_uart(main_blinky_smp 1)
