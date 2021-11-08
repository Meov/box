CC = gcc
TARGET:=mutex_test

OBJ:=main.o

$(TARGET):$(OBJ)
	$(CC) -o $(TARGET) $(OBJ) -lpthread
main.o:main.c
	$(CC) -c main.c 

clean:
	rm -rf $(TARGET) $(OBJ)
