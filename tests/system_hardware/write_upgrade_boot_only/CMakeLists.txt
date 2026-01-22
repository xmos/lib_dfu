cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(write_upgrade_boot_only)

# target: choose the target platform
set(APP_HW_TARGET XK-EVK-XU316)

# Includes and flags
set(APP_INCLUDES src)

set(APP_COMPILER_FLAGS
  -O2
  -g
  -lquadflash
  -fcmdline-buffer-bytes=256 # TODO do we need this?
  -report
  -Wall
  -Wextra
  -Wshadow
  -Wconversion
  -Wdiv-by-zero
  -Wfloat-equal
  -Wsign-compare
)

set(APP_DEPENDENT_MODULES lib_dfu "lib_xassert(4.3.2)" "lib_logging(3.4.0)")

set(XMOS_SANDBOX_DIR        ${CMAKE_CURRENT_LIST_DIR}/../../../..)

XMOS_REGISTER_APP()
