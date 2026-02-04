// Copyright 2019-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
//
// The original intention is for those functions to be the ones used by the
// DFU implementation, and those only
//
#ifndef __dfu_flash_h__
#define __dfu_flash_h__

#include <xccompat.h>

#define _Bool int
#include <stdbool.h>

enum flash_status {
  DFU_FLASH_BUSY = 1,
  DFU_FLASH_OK = 0,
  DFU_FLASH_OPEN_ERROR = -1,
  DFU_FLASH_ERASE_ERROR = -2,
  DFU_FLASH_GET_FACTORY_IMAGE_FAILED = -3,
  DFU_FLASH_READ_NO_IMAGE = -4,
  DFU_FLASH_BAD_PARAM = -5,
  DFU_FLASH_READ_ERROR = -6,
  DFU_FLASH_WRITE_ERROR = -7,
};

enum flash_status flash_cmd_enable_ports();

enum flash_status flash_cmd_disable_ports();

void DFUCustomFlashEnable();

void DFUCustomFlashDisable();

enum flash_status flash_cmd_init(void);

enum flash_status flash_cmd_deinit(void);




enum flash_status flash_erase_sector_async(unsigned address);

enum flash_status flash_write_page(const unsigned char page[], int length);

enum flash_status flash_finalise_write();

enum flash_status flash_read_page(REFERENCE_PARAM(unsigned char, data), int length);

bool flash_is_busy(void);

int flash_get_page_size(void);

int flash_get_size(void);


#include "dfu_flash_result.h"


bool flash_is_first_whole_page_in_sector(unsigned address);

bool flash_is_sector_erased(unsigned address);

enum flash_locate_boot_upgrade_slot_result
  flash_locate_boot_upgrade_slot(REFERENCE_PARAM(unsigned, address));

enum flash_locate_data_upgrade_slot_result
  flash_locate_data_upgrade_slot(REFERENCE_PARAM(unsigned, address));

enum flash_set_write_disable_result
  flash_set_write_disable(void);

bool flash_verify_page(unsigned address, const char page[]);

int flash_get_data_partition_base(void);

void flash_cmd_read_page(unsigned char *data);


//////

// init
// erase
// write
// read

#endif
