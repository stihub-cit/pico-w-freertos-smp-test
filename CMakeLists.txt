# Note that this file can be used as a top level CMakeLists.txt to build all the Demos (it includes
# them all, but they are usable in their own right, which is why no common work is done here)

cmake_minimum_required(VERSION 3.13)
set(EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico_w)
set(PICO_SDK_PATH "~/repo/pico-sdk")
# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

PROJECT(examples)
set( FREERTOS_KERNEL_PATH "~/repo/FreeRTOS/FreeRTOS/Source/" )

add_subdirectory(src)
