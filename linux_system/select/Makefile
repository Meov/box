CC = gcc
PROGRAM = test
OBJ := main.o
SRC := epoll.c

$(PROGRAM):$(OBJ)
	$(CC) -o $(PROGRAM) $(OBJ)

$(OBJ):$(SRC)
	$(CC) -o $(OBJ) -c $(SRC)

clean:
	rm -rf $(PROGRAM) $(OBJ)

