obj-m += helloworld.o

#replace kernel path below: 
KERNELDIR=/home/dtvl/liyw/SDK_REPO/bsp/kernel/kernel_414/dtb-linux-3.10.101
PWD := $(shell pwd)
 
all:     modules
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
 
clean:
	make -C $(KERNEL_VER) M=$(CURDIR) clean

