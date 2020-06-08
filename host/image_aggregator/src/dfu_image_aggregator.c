// Copyright (c) 2020, XMOS Ltd, All rights reserved
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "dfu_suffix.h"
#include "crc.h"
#include <string.h>

bool verbose = false;

int main(int argc, char **argv)
{
  unsigned vendor_id = 0, product_id = 0, bcd_device = 0;
  const char *in_boot_file = NULL, *in_data_file = NULL, *out_file = NULL;

  if (argc == 7 || argc == 6) {
    vendor_id = strtoul(argv[1], NULL, 0);
    product_id = strtoul(argv[2], NULL, 0);
  }
  if (argc == 6) {
    bcd_device = 0xFFFF;
    in_boot_file = argv[3];
    in_data_file = argv[4];
    out_file = argv[5];
  }
  if (argc == 7) {
    bcd_device = strtoul(argv[3], NULL, 0);
    in_boot_file = argv[4];
    in_data_file = argv[5];
    out_file = argv[6];
  }
  if (vendor_id == 0 || product_id == 0 || bcd_device == 0) {
    fprintf(stderr, "\
usage: dfu_suffix_generator VENDOR_ID PRODUCT_ID [BCD_DEVICE] BIN_BOOT_FILE_IN BIN_DATA_FILE_IN DFU_FILE_OUT\n\
\n\
       VENDOR_ID, PRODUCT_ID and BCD_DEVICE are non-zero 16bit values\n\
       decimal or hexadecimal format\n\
       0xFFFF means do not verify this field\n\
       0 is invalid value\n");
    exit(1);
  }

  unsigned crc = crc_init();
  char buf[1024];
  FILE * in_boot_stream = fopen(in_boot_file, "rb");
  FILE * in_data_stream = fopen(in_data_file, "rb");
  FILE * out_stream = fopen(out_file, "wb");
  size_t read = 0;

  if (in_boot_stream == NULL) {
    fprintf(stderr, "error: could not open input file %s\n", in_boot_file);
    exit(1);
  }
  if (in_data_stream == NULL) {
    fprintf(stderr, "error: could not open input file %s\n", in_data_file);
    exit(1);
  }

  if (out_stream == NULL) {
    fprintf(stderr, "error: could not open output file %s\n", out_file);
    exit(1);
  }

  // Get size of the data partition binary
  fseek(in_data_stream, 0, SEEK_END); // seek to end of file
  int data_bin_size = ftell(in_data_stream);
  fseek(in_data_stream, 0, SEEK_SET);

  while ((read = fread(buf, 1, sizeof(buf), in_data_stream)) != 0) {
          
    for (int i = 0; i < read; i++) {
      crc_step(&crc, buf[i]);
    }
  
    if (fwrite(buf, 1, read, out_stream) != read) {
      fprintf(stderr, "error: I/O write and read mismatch\n");
      exit(1);
    }
  }

  fclose(in_data_stream);

  while ((read = fread(buf, 1, sizeof(buf), in_boot_stream)) != 0) {
          
    for (int i = 0; i < read; i++) {
      crc_step(&crc, buf[i]);
    }
  
    if (fwrite(buf, 1, read, out_stream) != read) {
      fprintf(stderr, "error: I/O write and read mismatch\n");
      exit(1);
    }
  }
  fclose(in_boot_stream);

  crc = crc_finish(crc);
  
  struct dfu_suffix suffix = {
    .crc = crc,
    .suffix_length = sizeof(struct dfu_suffix),
    .signature = DFU_SIGNATURE,
    .bcd_dfu = DFU_BCD,
    .data_bin_size = data_bin_size,
    .vendor_id = vendor_id,
    .product_id = product_id,
    .bcd_device = bcd_device
  };
  printf("Suffix values:\n\tcrc = 0x%08x,\n\tsuffix_length = %d,\n\tsignature = 0x%08x,\n\tbcd_dfu = 0x%04x,\n\tdata_bin_size = %d,\n\tvendor_id = 0x%04x,\n\tproduct_id =  0x%04x,\n\tbcd_device =  0x%04x.\n",
    suffix.crc,
    suffix.suffix_length,
    (suffix.signature[2]<<16) + (suffix.signature[1]<<8) + suffix.signature[0],
    suffix.bcd_dfu,
    suffix.data_bin_size,
    suffix.vendor_id,
    suffix.product_id,
    suffix.bcd_device);
  char reversed[sizeof(struct dfu_suffix)];
  for (int i = 0; i < sizeof(reversed); i++) {
    reversed[i] = ((char*)&suffix)[sizeof(reversed) - 1 - i];
  }
  if (fwrite(reversed, sizeof(reversed), 1, out_stream) != 1) {
    fprintf(stderr, "error: I/O write of suffix invalid return value\n");
    exit(1);
  }

  return 0;
}
