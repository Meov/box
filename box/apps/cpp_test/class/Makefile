#CFLAGS := -g -Wall -Werror -std=c++11
CFLAGS := -g -Wall -std=c++11
CC := g++
TARGET := test

$(TARGET) : ./*.cpp
	$(CC) $(CFLAGS)  -o $@ $<

clean:
	rm $(TARGET)
