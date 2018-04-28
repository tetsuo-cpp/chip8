cmake_minimum_required(VERSION 3.9)
project(chip8)

# Setup SDL2.
find_package(SDL2 REQUIRED)
include_directories(chip8 ${SDL2_INCLUDE_DIRS})

# Setup GTest/GMock.
# This section is copied from the Google Test README.

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Compiler flags.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

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
