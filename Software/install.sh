#!/usr/bin/env bash
SRC_DIR=$(pwd)
ETC_OPQ_SETTINGS="/etc/opq/settings.set"
ETC_OPQ_DIR="/etc/opq/"

#Check for settings.set
if [ ! -f ${ETC_OPQ_SETTINGS} ]; then
	echo ${ETC_OPQ_SETTINGS} not found
	echo Copying template.set to ${ETC_OPQ_SETTINGS}
	#uses template.set if settings.set doesn't exist
	sudo mkdir -p ${ETC_OPQ_DIR} && sudo cp template.set ${ETC_OPQ_SETTINGS} 
fi

#Copy Triggering Binary
echo Copying Triggering Binary to /usr/local/bin/
sudo cp ./Triggering/build/Triggering /usr/local/bin/

#Install Acquisition
echo Installing Acquisition Software
cd ./Acquisition/
	sudo python setup.py install
cd ${SRC_DIR}


#Create opquser
echo Creating account opquser if it does not exist
id -u opquser &>/dev/null || sudo useradd opquser
