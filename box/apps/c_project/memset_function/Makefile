CC = gcc
TARGET = test
OBJ = main.o
SRC = main.c

$(TARGET):$(OBJ)
	$(CC) -o $(TARGET) $(OBJ)
$(OBJ):$(SRC)
	$(CC) -o $@ -c $<

.PHONY : clean
clean:
	rm -rf $(TARGET) $(OBJ)

