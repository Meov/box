# Makefile for dtvl-pltest
exe = dtvl-pltest
objects = src/main.o \
		  src/equipment.o \
		  src/utils.o \
		  src/gpio_operate.o
objects += src/HSM/src/xdja_platform.o

CC = $(CROSS_COMPILE)gcc 
CFLAGS = -g -Wall -O2
CFLAGS += -I ./inc/
CFLAGS += -I ./src/HSM/inc/
CLDFLAGS = -L ./src/HSM/ -lxdjakey-332k-0813 -lrt

.PHONY: all

all: $(objects)
	$(CC) -lpthread -o $(exe) $(objects) $(CLDFLAGS)
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean 
clean:
	rm $(objects) $(exe)
