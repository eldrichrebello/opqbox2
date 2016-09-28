#!/usr/bin/env bash

#Building kernel files
cd kernel
	make
#	sudo make install
cd ..

#Build protocol files
mkdir -p Acquisition/proto
protoc -I=proto/ --cpp_out=Triggering/proto proto/opq.proto
protoc -I=proto/ --python_out=Acquisition/ proto/opq.proto

cd Acquisition
	make 
	sudo make install
cd ..
