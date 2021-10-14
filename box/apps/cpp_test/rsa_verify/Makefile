TARGET := verify_it
CC := g++
CXXFLAGS = -std=c++11 -g
LIBS := -lssl -lcrypto

all:$(TARGET)

$(TARGET) : ./*.cpp ./*.hxx
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm -rf $(TARGET)
