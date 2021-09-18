#CFLAGS := -g -Wall -Werror -std=c++11
CFLAGS := -g -Wall -std=c++11
CC := g++

TARGET := test simple main

all:$(TARGET)

test: test.cxx
	$(CC) $(CFLAGS)  -o $@ $<

simple: simple.cxx
	$(CC) $(CFLAGS)  -o $@ $<

main: main.cxx
	$(CC) $(CFLAGS)  -o $@ $<
clean:
	rm $(TARGET)
