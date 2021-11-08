CC = g++
CXXFLAGS += -std=c++11 -g -Wall -Wextra -Werror  \
            -Wno-error=unused-parameter \
            -fsanitize=address -fno-omit-frame-pointer
LIBS += -lubsan

TARGET = string_test
DEPS = mystring.hxx
OBJ = main.o mystring.o

$(TARGET): $(OBJ)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LIBS)
 
%.o: %.cpp $(DEPS)
	$(CC) $(CXXFLAGS)  -c $< -o $@ $(LIBS)
 
clean:
	rm -rf $(OBJ) $(TARGET)
