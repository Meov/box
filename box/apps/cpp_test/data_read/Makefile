CC = g++
CXXFLAGS += -std=c++11 -g -Wall -fsanitize=address -fno-omit-frame-pointer -Wextra -Werror -Wno-error=unused-parameter
LIBS += -lubsan

TARGET = test
DEPS = data.hxx
OBJ = main.o data.o

#	$(CC) $(CXXFLAGS) -o $@ $^ $(LIBS)
#
#
#
$(TARGET): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp $(DEPS)
	$(CC) $(CXXFLAGS)  -c $< -o $@ $(LIBS)
 
clean:
	rm -rf $(OBJ) $(TARGET)
