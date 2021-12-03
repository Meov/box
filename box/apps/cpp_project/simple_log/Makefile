TARGET := simple_log
CC := g++
#CC := aarch64-linux-gnu-g++
CXXFLAGS = -std=c++11 -g
LIBS := 

all:$(TARGET)

$(TARGET) : ./*.cpp ./*.hpp
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(TARGET)
