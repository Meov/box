CC = gcc
PROGRAM = test
OBJ := main.o
SRC := main.c

$(PROGRAM):$(OBJ)
	$(CC) -o $(PROGRAM) $(OBJ)

$(OBJ):$(SRC)
	$(CC) -o $(OBJ) -c $(SRC)

clean:
	rm -rf $(PROGRAM) $(OBJ)

