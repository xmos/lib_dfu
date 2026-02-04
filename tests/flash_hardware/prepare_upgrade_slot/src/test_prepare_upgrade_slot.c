// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <platform.h>
#include <stdint.h>
#include <stdio.h>
#include <unity.h>
#include <xcore/hwtimer.h>

#include "dfu_flash.h"

/* Main args */
FILE* upgrade_file = NULL;
uint32_t erase_timing_threshold_ms = 0;
uint32_t write_timing_threshold_ms = 0;

static hwtimer_t prepare_timer;

void setUp() { prepare_timer = hwtimer_alloc(); }

void tearDown() {
  flash_cmd_deinit();
  hwtimer_free(prepare_timer);
}

int write(hwtimer_t runtime, FILE* file) {
  uint8_t page[256] = {0};

  uint32_t start_runtime = hwtimer_get_time(runtime);
  uint32_t max_runtime = start_runtime + (60UL * XS1_TIMER_HZ);  // 60s
  uint32_t running;
  uint32_t wr_run_time = 0;
  enum flash_status wr_status = DFU_FLASH_OK;
  size_t read_total = 0;
  size_t read = fread(page, 1, sizeof(page), file);
  do {
    read_total += read;
    // assert(read >= 0 && read <= block_size);
    uint32_t wr_start = hwtimer_get_time(runtime);
    wr_status = flash_write_page(page, sizeof(page));
    uint32_t wr_end = hwtimer_get_time(runtime);
    wr_run_time += (wr_end - wr_start);

    running = hwtimer_get_time(runtime);
    // Reading file over xscope is slow - so only time the write operation
    read = fread(page, 1, sizeof(page), file);
  } while ((wr_status == DFU_FLASH_OK) && !feof(file) && !hwtimer_time_after(running, max_runtime));

  printf("Read total %zu\n", read_total);
  printf("Write time: %0.3fs\n", (float)wr_run_time / (float)XS1_TIMER_HZ);  // Typically ~30ms seconds
  TEST_ASSERT_TRUE(wr_run_time < (write_timing_threshold_ms * XS1_TIMER_KHZ));
  return wr_status;
}

// TODO - Ideally test with factory-only and upgrade image present

void test_dfu_flash_prepare_slot_reports_OK(void) {
  int status = flash_cmd_init();
  TEST_ASSERT_EQUAL(DFU_FLASH_OK, status);

  uint32_t start_runtime = hwtimer_get_time(prepare_timer);
  uint32_t max_runtime = start_runtime + (60UL * XS1_TIMER_HZ);  // 60s
  uint32_t running;
  int erase = DFU_FLASH_BUSY;

  do {
    erase = flash_erase_sector_async(0);
    hwtimer_delay(prepare_timer, 10UL * XS1_TIMER_KHZ);  // 10ms

    running = hwtimer_get_time(prepare_timer);
  } while (erase == DFU_FLASH_BUSY && !hwtimer_time_after(running, max_runtime));

  printf("Erase time: %0.2fs\n", (float)(running - start_runtime) / XS1_TIMER_HZ);  // Typically ~7 seconds
  TEST_ASSERT_TRUE((running - start_runtime) > (4000 * XS1_TIMER_KHZ));
  TEST_ASSERT_TRUE((running - start_runtime) < (erase_timing_threshold_ms * XS1_TIMER_KHZ));
  TEST_ASSERT_EQUAL(DFU_FLASH_OK, erase);
}

void test_dfu_flash_write_reports_OK(void) {
  TEST_ASSERT_NOT_NULL(upgrade_file);

  int status = flash_cmd_init();
  TEST_ASSERT_EQUAL(DFU_FLASH_OK, status);

  int wr_status = write(prepare_timer, upgrade_file);

  enum flash_status final = flash_finalise_write();

  TEST_ASSERT_EQUAL(DFU_FLASH_OK, wr_status);
  TEST_ASSERT_EQUAL(DFU_FLASH_OK, final);
  // Upgrade image is now valid
}
