#!/usr/bin/env bash
TRIGGERING_BUILD_DIR="./Triggering/build/"
SRC_DIR=$(pwd)

#Building kernel files
cd kernel
	echo Building kernel driver.
	make >> /dev/null
cd ${SRC_DIR}

#Build protocol files
mkdir -p Triggering/proto
protoc -I=proto/ --cpp_out=Triggering/proto proto/opq.proto
protoc -I=proto/ --python_out=Acquisition/ proto/opq.proto

if [ ! -d ${TRIGGERING_BUILD_DIR} ]; then
	echo Creating a build directory for the Triggering daemon ${TRIGGERING_BUILD_DIR}
	mkdir -p ${TRIGGERING_BUILD_DIR}
	cd  ${TRIGGERING_BUILD_DIR}
	cmake ${SRC_DIR}/Triggering
	cd ${SRC_DIR}
fi

cd  ${TRIGGERING_BUILD_DIR}
	make		
cd ${SRC_DIR}
