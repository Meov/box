TARGRT = serial
OBJ = main.o
CROSS_COMPIL = arm-linux-gnueabi-
CC:= $(CROSS_COMPIL)gcc
LD:= $(CROSS_COMPIL)ld


$(TARGRT): $(OBJ)
	$(CC) -o $(TARGRT) $(OBJ)

%.o : %.c
	$(CC) -o $@ -c $^

.PHONY: clean
clean:
	rm -rf $(TARGRT) $(OBJ)
