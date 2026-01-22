cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(address_bounds_erase_test)

include(${CMAKE_CURRENT_LIST_DIR}/../../../deps.cmake)

# Includes and flags
set(APP_INCLUDES ../../../../lib_dfu/src)

# target: choose the target platform
set(APP_HW_TARGET XK-EVK-XU316)

# Test will run in simulator, so do not include "-lquadflash" here, as the library functions are stubbed in the test file.
set(APP_COMPILER_FLAGS
  -O2
  -g
  -report
  -Wall
  -Wextra
  -Wshadow
  -Wconversion
  -Wdiv-by-zero
  -Wfloat-equal
  -Wsign-compare
)

set(XMOS_SANDBOX_DIR        ${CMAKE_CURRENT_LIST_DIR}/../../../../..)

XMOS_REGISTER_APP()
