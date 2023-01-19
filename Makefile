CC=gcc
CFLAGS=-g -Wall
OBJ=main.o

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

build: clean main

run: build
	bin/main

main: ${OBJ_DIR}/${OBJ}
	${CC} $^ -o ${BIN_DIR}/$@ ${CFLAGS}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	${CC} -c $< -o $@ ${CFLAGS}

clean:
	@rm -f ${OBJ_DIR}/*.o
	@rm -f ${BIN_DIR}/main