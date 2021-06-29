OBJECTS = src/csvToBinary.c src/dataModel.c src/operations.c src/printer.c src/selectWhere.c src/bTreeDataModel.c src/stringTable.c src/binaryReader.c src/binaryWriter.c src/bTree.c src/binaryHeaders.c src/utils.c

all:
	gcc src/main.c ${OBJECTS} -o programaTrab -Wall

run:
	./programaTrab

# Windows-specific
d:
	gcc -ggdb3 src/main.c ${OBJECTS} -o programaTrab.exe -Wall
	gdb programaTrab.exe

r:
	gcc src/main.c ${OBJECTS} -o programaTrab.exe -Wall
	programaTrab.exe

clean:
	rm ./*bin ./*csv programaTrab.exe