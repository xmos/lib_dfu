// Copyright 2011-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef DFU_DEFAULT_CONF_H
#define DFU_DEFAULT_CONF_H

#ifdef __dfu_conf_h_exists__
#warning "Custom dfu_conf.h found, default configuration will be overridden"
#include "dfu_conf.h"
#endif

/** Number of bytes transferred in each DFU packet on the given physical transport */
#ifndef DFU_TRANSFER_SIZE_BYTES
#define DFU_TRANSFER_SIZE_BYTES 64
#endif

/** Number of bytes in a flash page for the target device */
#ifndef DFU_FLASH_PAGE_SIZE_BYTES
#define DFU_FLASH_PAGE_SIZE_BYTES 256
#endif

/** Number of DFU packets per flash page */
#ifndef NUM_DFU_PAGES_PER_FLASH_PAGE
#define NUM_DFU_PAGES_PER_FLASH_PAGE (DFU_FLASH_PAGE_SIZE_BYTES / DFU_TRANSFER_SIZE_BYTES)
#if DFU_TRANSFER_SIZE_BYTES > DFU_FLASH_PAGE_SIZE_BYTES
#error "DFU_TRANSFER_SIZE_BYTES must not be greater than DFU_FLASH_PAGE_SIZE_BYTES"
#endif
#endif

#endif /* DFU_DEFAULT_CONF_H */
