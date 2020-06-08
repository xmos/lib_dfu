# Copyright (c) 2020, XMOS Ltd, All rights reserved
DUT_PATH = ../../../../host/image_aggregator
EXECUTABLE = dfu_image_aggregator
MKDIR ?= mkdir
CP ?= cp

all: dirs prebuild build

dirs:
	@${MKDIR} -p bin

prebuild:
	cd ${DUT_PATH} ; cmake .
	make -C ${DUT_PATH} clean

build:
	make -C ${DUT_PATH}
	${CP} ${DUT_PATH}/bin/${EXECUTABLE} bin

clean:
	${RM} ${EXECUTABLE}
	make -C ${DUT_PATH} clean
	${RM} ${DUT_PATH}/CMakeCache.txt ${DUT_PATH}/makefile
