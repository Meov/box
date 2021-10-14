CROSS_COMPILE ?= arm-linux-gnueabi-
TARGET = tcm_spi

CROSS_COMPILE ?= 


ARCH：= arm
CC:= $(CROSS_COMPILE)gcc
LD:= $(CROSS_COMPILE)ld
OBJCOPY	:= $(CROSS_COMPILE)objcopy
OBJDUMP	:= $(CROSS_COMPILE)objdump

#单一目录下的Makefile
OBJ = dmt_spi.o
${TARGET} : ${OBJ}
	$(CC) -o ${TARGET} ${OBJ}
dmt_spi.o:dmt_spi.c dmt_spi.h
	$(CC) -c dmt_spi.c
#crc.o:crc.c crc.h
#	gcc -c crc.c
#$@--目标文件，$^--所有的依赖文件，$<--第一个依赖文件
#%.o : %.c
#	$(CC) -o $@  -c $^ 

#${TARGET} : ${OBJ}
#	gcc -o ${TARGET} ${OBJ}

#%.o : %.c
#	gcc -o $@  -c $^ 


.PHONY: clean
clean:
	rm -rf ${OBJ} ${TARGET}
