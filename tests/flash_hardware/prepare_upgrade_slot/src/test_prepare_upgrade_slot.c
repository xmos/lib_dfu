// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <platform.h>
#include <stdint.h>
#include <unity.h>
#include <xcore/hwtimer.h>

#include "dfu_flash.h"


void setUp() {}
void tearDown() { flash_cmd_deinit(); }

void delay() {
  hwtimer_t delay = hwtimer_alloc();
  hwtimer_delay(delay, 10000000);  // 100ms
  hwtimer_free(delay);
}

void holdOff() {
  hwtimer_t delay = hwtimer_alloc();
  while (flash_is_busy()) {
    hwtimer_delay(delay, 10000000);  // 100ms
  }
  hwtimer_free(delay);
}

void test_dfu_flash_init_reports_OK(void) {
  int status = flash_cmd_init();
  TEST_ASSERT_EQUAL(DFU_FLASH_OK, status);
}

void test_dfu_flash_after_init_is_not_busy(void) {
  int status = flash_cmd_init();
  TEST_ASSERT_EQUAL(DFU_FLASH_OK, status);
  TEST_ASSERT_FALSE(flash_is_busy());
}

void test_dfu_flash_erase_sector_async_reports_OK(void) {
  int status = flash_cmd_init();
  TEST_ASSERT_EQUAL(DFU_FLASH_OK, status);
  status = flash_erase_sector_async(0);

  // let the flash erase complete
  delay();

  TEST_ASSERT_EQUAL(DFU_FLASH_BUSY, status);
}
