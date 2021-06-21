CROSS_COMPILE ?= arm-linux-gnueabi-
TARGET = dmd_tcm_test

CROSS_COMPILE ?= 


ARCH：= arm
CC:= $(CROSS_COMPILE)gcc
LD:= $(CROSS_COMPILE)ld
OBJCOPY	:= $(CROSS_COMPILE)objcopy
OBJDUMP	:= $(CROSS_COMPILE)objdump


OBJ_DIR  := obj

$(shell if [ ! -e $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi)

INCDIRS := serial_dir\
		crc\
		app\
		tcm_driver

SRCDIRS := serial_dir \
		app\
		tcm_driver\
		crc\

INCLUDE	:= $(patsubst %, -I %, $(INCDIRS))

CFILES	:=$(foreach dir, $(SRCDIRS),$(wildcard $(dir)/*.c))

#print:
#	@echo CFILES=$(CFILES)
CFILENDIR	:= $(notdir  $(CFILES))
VPATH                   := $(SRCDIRS)
OBJS	:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))

$(TARGET) :$(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

$(OBJS) : $(OBJ_DIR)/%.o : %.c
	$(CC) $(INCLUDE) -o $@ -c $<


#单一目录下的Makefile
#OBJ = test.o crc.o
#${TARGET} : ${OBJ}
#	$(CC) -o ${TARGET} ${OBJ}
#test.o:test.c
#	gcc -c test.c
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
	rm -rf $(OBJS) $(TARGET) $(OBJ_DIR)
