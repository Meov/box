
CFLAGS += -fPIC -Wall -std=gnu99

TARGET = libapi.so

API_PATH = $(shell pwd)
SHARE_LIB_PATH = /usr/lib

SRCS = $(API_PATH)/src/api.c

INC = -I$(API_PATH)/inc

DLIBS = #-lapi
LDFLAGS = #-L$(API_PATH)/api

RPATH = #-Wl,-rpath=$(SHARE_LIB_PATH) 

OBJS = $(SRCS:.c=.o)

$(TARGET):$(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS) $(DLIBS) $(RPATH)

%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	rm -rf $(TARGET) $(OBJS)
