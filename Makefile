OBJECTS = src/csvToBinary.c src/dataModel.c src/operations.c src/printer.c src/selectWhere.c src/bTreeCache.c src/stringTable.c src/binaryReader.c src/binaryWriter.c src/bTree.c src/binaryHeaders.c src/utils.c

all:
	gcc src/main.c ${OBJECTS} -o programaTrab -Wall

run:
	make all
	programaTrab.exe

#run:
#	./programaTrab

clean:
	rm ./*bin ./*csv programaTrab.exe