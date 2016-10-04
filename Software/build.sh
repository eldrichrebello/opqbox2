#!/usr/bin/env bash
TRIGGERING_BUILD_DIR="./Triggering/build/"
TRIGGERING_DOCS_DIR="./docs/Triggering"
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


#Create Docs Directory and Generate Documentation
if [ ! -d ${TRIGGERING_DOCS_DIR} ]; then
	echo Creating a docs directory for Triggering ${TRIGGERING_DOCS_DIR}
	mkdir -p ${TRIGGERING_DOCS_DIR}
fi

cd ${TRIGGERING_DOCS_DIR}
	echo Creating Doxygen Config file
	doxygen -g
	echo Editing Project Name, Input Source, and Disabling Latex Output
	sed -i 's;PROJECT_NAME           = "My Project";PROJECT_NAME           = "Triggering";' Doxyfile
	sed -i 's;INPUT                  =;INPUT                  = ../../Triggering/lib;' Doxyfile
	sed -i 's;GENERATE_LATEX         = YES;GENERATE_LATEX         = NO;' Doxyfile
	doxygen Doxyfile
cd ${SRC_DIR}
