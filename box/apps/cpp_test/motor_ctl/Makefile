#CC = g++
CC := arm-linux-gnueabihf-g++
CXXFLAGS += -std=c++11 -g -Wall -Wextra -Werror  \
            #-Wno-error=unused-parameter \
            #-fsanitize=address -fno-omit-frame-pointer
LIBS += -lpthread
TARGET := motor

INCDIRS := ./src/ \
		./src/common

SRCDIRS := src/ \
		./src/common

INCLUDE := $(patsubst %, -I %, $(INCDIRS))

CPPFILES :=$(foreach dir, $(SRCDIRS),$(wildcard $(dir)/*.cpp))

CFILENDIR := $(notdir  $(CPPFILES))

#指定了make的搜索路径，使用 := 表示当前VPATH的路径 当前VPATH: SRCDIRS := src/
VPATH := $(SRCDIRS)
OBJS := $(patsubst %, obj/%, $(CFILENDIR:.cpp=.o))

(TARGET) :$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CXXFLAGS) $(LIBS)

$(OBJS) : obj/%.o : %.cpp
	$(CC) $(INCLUDE) -o $@ -c $< $(CXXFLAGS) $(LIBS)

clean:
	rm -rf $(OBJS) $(TARGET)
