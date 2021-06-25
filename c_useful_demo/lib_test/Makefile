OBJ := main.o
TARGET := main
CC := gcc


$(TARGET) : $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) -L./testlib -ltest
$(OBJ) : main.c
	$(CC) -c main.c -o  $(OBJ)

clean:
	rm $(TARGET)  $(OBJ)
