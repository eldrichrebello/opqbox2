obj-m  += opq.o
opq-objs := opq_main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f opq.dtbo

install:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules_install

opq.dtbo: opq.dts
	dtc -I dts -O dtb -o opq.dtbo opq.dts
