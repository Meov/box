TARGET := timeslice
CC := gcc
CXXFLAGS = -g 

all:$(TARGET)

$(TARGET) : ./*.c ./*.h
	$(CC) $(CXXFLAGS) -o $@ $^ -lpthread

clean:
	rm -rf $(TARGET)

