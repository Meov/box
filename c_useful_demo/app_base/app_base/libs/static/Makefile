
CFLAGS += -fPIC -Wall -std=gnu99 

TARGET = libapi.a

API_PATH = $(shell pwd)

SRCS = $(API_PATH)/src/api.c

INC = -I$(API_PATH)/inc

DLIBS = #-lapi
LDFLAGS = #-L$(API_PATH)/api

OBJS = $(SRCS:.c=.o)

$(TARGET):$(OBJS)
	ar rcs $@ $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	rm -rf $(TARGET) $(OBJS)
