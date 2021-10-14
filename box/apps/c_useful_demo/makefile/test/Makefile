SRC=$(wildcard *.c ./sub/*.c)


DIR=$(notdir $(SRC))


OBJ=$(patsubst %.c,%.o,$(DIR))

OBJ_1=$(patsubst %.c,%.o,$(SRC))

print_SRC:
	@echo SRC = $(SRC)
	@echo DIR = $(DIR)
	@echo OBJ = $(OBJ)
	@echo OBJ_1 = $(OBJ_1)

