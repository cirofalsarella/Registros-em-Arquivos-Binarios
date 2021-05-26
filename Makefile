OBJECTS = src/csvToBinary.c src/dataModel.c src/operations.c src/printer.c src/stringTable.c src/readBinary.c src/selectWhere.c

all:
	gcc src/main.c ${OBJECTS} -o programaTrab.exe -Wall
	
run:
	make all
	./programaTrab.exe

clean:
	rm ./*bin ./*csv programaTrab.exe