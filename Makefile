OBJECTS = src/dataModel.c src/operations.c src/printer.c src/bTreeDataModel.c src/binaryReader.c src/binaryWriter.c src/bTree.c src/binaryHeaders.c src/utils.c

all:
	gcc -ggdb3 src/main.c ${OBJECTS} -o programaTrab -Wall

run:
	./programaTrab

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./programaTrab

gdb:
	gdb programaTrab


# Windows-specific
c:
	gcc -ggdb3 src/main.c ${OBJECTS} -o programaTrab.exe -Wall

d:
	gcc -g src/main.c ${OBJECTS} -o programaTrab.exe -Wall
	gdb programaTrab.exe

r:
	gcc src/main.c ${OBJECTS} -o programaTrab.exe -Wall
	programaTrab.exe

clean:
	rm ./*bin ./*csv programaTrab.exe