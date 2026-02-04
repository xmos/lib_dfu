// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <platform.h>
#include <stdint.h>
#include <stdio.h>
#include <unity.h>
#include <xcore/hwtimer.h>

#include "dfu_flash.h"

static hwtimer_t runtime;

void setUp() { runtime = hwtimer_alloc(); }

void tearDown() {
  flash_cmd_deinit();
  hwtimer_free(runtime);
}

// TODO - Ideally test with factory-only and upgrade image present

void test_dfu_flash_init_reports_OK(void) {
  int status = flash_cmd_init();
  TEST_ASSERT_EQUAL(DFU_FLASH_OK, status);

  uint32_t start_runtime = hwtimer_get_time(runtime);
  uint32_t max_runtime = start_runtime + (60UL * XS1_TIMER_HZ);  // 60s
  uint32_t running;
  int erase = DFU_FLASH_BUSY;
  do {
    erase = flash_erase_sector_async(0);
    hwtimer_delay(runtime, 10UL * XS1_TIMER_KHZ);  // 10ms

    running = hwtimer_get_time(runtime);
  } while (erase == DFU_FLASH_BUSY && !hwtimer_time_after(running, max_runtime));

  printf("Erase time: %lu\n", running - start_runtime); // Typically ~7 seconds
  TEST_ASSERT_EQUAL(DFU_FLASH_OK, erase);
}

// void test_dfu_flash_failing_test(void) {
//   TEST_ASSERT_TRUE(0);
// }
