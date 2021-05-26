OBJECTS = src/csvToBinary.c src/dataModel.c src/operations.c src/printer.c src/selectWhere.c src/stringTable.c src/binaryReader.c src/binaryWriter.c src/binaryHeaders.c src/utils.c

all:
	gcc src/main.c ${OBJECTS} -o programaTrab.exe -Wall
	
run:
	make all
	./programaTrab.exe

clean:
	rm ./*bin ./*csv programaTrab.exe