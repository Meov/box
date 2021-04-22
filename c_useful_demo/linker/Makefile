TARGET = sum_add
OBJ = sum.o main.o

$(TARGET) : $(OBJ)
	gcc -o $(TARGET) $(OBJ)

main.o : main.c
	gcc -c main.c
sum.o : sum.c
	gcc -c sum.c	
#$(OBJ) : main.c sum.c
#	gcc -c $< -o $@

clean:
	rm -rf $(TARGET)  $(OBJ)
