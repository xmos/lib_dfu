DFU Image Aggregator
====================

Typical usage
-------------

Inputs (boot.bin and data.bin below) are a boot image produced by xflash and a data partition
image produced by the appropriate generator utility (Flash data partition
library). Output (final.bin below) is a version of the aggregated files with DFU suffix
appended.

    dfu_image_aggregator 0x20B1 0x0014 0x0102 boot.bin data.bin final.bin
