#CFLAGS := -g -Wall -Werror -std=c++11
CFLAGS := -g -Wall -std=c++11
CC := g++

TARGET := test simple main

all:$(TARGET)

test: test.cpp
	$(CC) $(CFLAGS)  -o $@ $<

simple: simple.cpp
	$(CC) $(CFLAGS)  -o $@ $<

main: main.cpp
	$(CC) $(CFLAGS)  -o $@ $<
clean:
	rm $(TARGET)
