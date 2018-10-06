cmake_minimum_required(VERSION 3.9)
project(chip8)

# Compiler flags.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# Setup SDL2.
find_package(SDL2 REQUIRED)
include_directories(chip8 ${SDL2_INCLUDE_DIRS})

# Setup GTest/GMock.
add_subdirectory(
  "${PROJECT_SOURCE_DIR}/external/googletest"
  "external/googletest"
  )

set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

# Build emulator binary.
set(
  CHIP8_SOURCE_FILES
  lib/chip8_lib/chip8.cc
  lib/chip8_lib/chip8.h
  lib/chip8_lib/clock.cc
  lib/chip8_lib/clock.h
  lib/chip8_lib/controller.cc
  lib/chip8_lib/controller.h
  lib/chip8_lib/cpu.cc
  lib/chip8_lib/cpu.h
  lib/chip8_lib/display.cc
  lib/chip8_lib/display.h
  lib/chip8_lib/interfaces.h
  lib/chip8_lib/random.h
  lib/chip8_lib/rom.cc
  lib/chip8_lib/rom.h
  src/main.cc
  )

add_executable(chip8 ${CHIP8_SOURCE_FILES})

target_link_libraries(chip8 ${SDL2_LIBRARIES})
target_include_directories(chip8 PRIVATE lib)

# Build unit test binary.
set(
  CHIP8_TEST_SOURCE_FILES
  lib/chip8_lib/cpu.cc
  lib/chip8_lib/cpu.h
  lib/chip8_lib/interfaces.h
  lib/chip8_test_lib/mocks.h
  lib/chip8_test_lib/test_cpu.h
  test/cpu_test.cc
  test/test_main.cc
  )

add_executable(chip8_test ${CHIP8_TEST_SOURCE_FILES})

target_link_libraries(chip8_test gtest gmock)
target_include_directories(chip8_test PRIVATE lib)
