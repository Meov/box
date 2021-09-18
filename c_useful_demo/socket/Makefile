OBJ1 := server
OBJ2 := client
SUR_OBJ1 := server.o
SUR_OBJ2 := client.o
CC := gcc
AOBJS := $(OBJ1) $(OBJ2)
objects := $(SUR_OBJ1) $(SUR_OBJ2)

.PHONY:all
all:$(OBJ1) $(OBJ2)

$(OBJ1):$(SUR_OBJ1)
	$(CC) -o $@ $^
$(OBJ2):$(SUR_OBJ2)
	$(CC) -o $@ $^

.PHONY:clean
clean:
	echo $(AOBJS)
	rm -rf $(AOBJS) $(objects)

