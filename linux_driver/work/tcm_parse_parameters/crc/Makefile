TARGET = uart_test
OBJ = test.o crc.o

${TARGET} : ${OBJ}
	gcc -o ${TARGET} ${OBJ}
test.o:test.c
	gcc -c test.c
crc.o:crc.c crc.h
	gcc -c crc.c

clean:
	rm -rf ${OBJ} ${TARGET}
