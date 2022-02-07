TARGET := test
CXXFLAGS := --std=c++11 -pthread

$(TARGET) : main.cpp
	g++ -o $@ $< $(CXXFLAGS)

clean:
	rm *.o $(TARGET)
