cmake_minimum_required(VERSION 3.19)
project(reliability_lib C)

if(NOT UNIX)
  message(FATAL_ERROR "Only UNIX platforms are currently supported")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)

# set riscv-32 toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv32)
set(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)

message(STATUS "[INFO]: toolchain is successfully set.")

# create build dir
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/build)
  file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
endif()

message(STATUS "[INFO]: build dir is successfully created.")

option(TARGET_PLATFORM "Target platform: qemu | baremetal | freertos" "qemu")

# Путь к исходникам
include_directories(
  ${CMAKE_SOURCE_DIR}/tools/
  ${CMAKE_SOURCE_DIR}/tools/reliability_mechanisms/error_detection/
  ${CMAKE_SOURCE_DIR}/tools/error_monitor/

  ${CMAKE_SOURCE_DIR}/platform/qemu/
  ${CMAKE_SOURCE_DIR}/platform/adapter/

  ${CMAKE_SOURCE_DIR}/snippets/
)

# Собираем библиотеку
file(GLOB LIB_SRC
  ${CMAKE_SOURCE_DIR}/tools/reliability_mechanisms/error_detection/*.c
  ${CMAKE_SOURCE_DIR}/tools/error_monitor/*.c

  ${CMAKE_SOURCE_DIR}/platform/qemu/*.c
  ${CMAKE_SOURCE_DIR}/platform/adapter/*.c
  
  ${CMAKE_SOURCE_DIR}/snippets/*.c
)

if(TARGET_PLATFORM STREQUAL "qemu")
  message(STATUS "[INFO]: Building for QEMU")
  set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/platform/qemu/qemu_project.ld)
  set(STARTUP_FILE ${CMAKE_SOURCE_DIR}/platform/qemu/startup.s)

elseif(TARGET_PLATFORM STREQUAL "baremetal")
  message(STATUS "[INFO]: Building for Baremetal")

elseif(TARGET_PLATFORM STREQUAL "freertos")
  message(STATUS "[INFO]: Building for FreeRTOS")
  
else()
  message(FATAL_ERROR "Unknown platform: ${TARGET_PLATFORM}")
endif()

message(STATUS "[INFO]: lib is build successfully.")

add_library(reliability STATIC ${LIB_SRC})

# Собираем пример (main.c)
add_executable(main
    ${CMAKE_SOURCE_DIR}/main.c
)

message(STATUS "[INFO]: snippet is build successfully.")

target_link_libraries(main reliability)

message(STATUS "[INFO]: project is linked successfully.")

target_compile_options(main PRIVATE -g -O0 -ffreestanding -nostartfiles -Wall -Wextra -march=rv32imc -mabi=ilp32)
target_link_options(main PRIVATE -nostartfiles -T${LINKER_SCRIPT} -Wl,-Map=main.map)

message(STATUS "[INFO]: Project is configured for RISC-V successfully.")
