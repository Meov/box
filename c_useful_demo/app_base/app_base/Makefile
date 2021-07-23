MAIN_PATH = $(shell pwd)

CFLAGS = -fPIC -Wall -std=gnu99 -pthread 

TARGET = app

SRCS = $(MAIN_PATH)/src/main.c

INC = -I$(MAIN_PATH)/include

DLIBS = #-llib
LDFLAGS = -L$(MAIN_PATH)/libs/extra

RPATH = -Wl,-rpath=$(MAIN_PATH)/depend/usr/local/lib/

OBJS = $(SRCS:.c=.o)

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(DLIBS) $(RPATH) 

%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	rm -rf $(TARGET) $(OBJS)
