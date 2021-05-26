OBJECTS = src/csvToBinary.c src/dataModel.c src/operations.c src/printer.c src/stringTable.c src/readBin.c src/selectWhere.c src/funcao-fornecida.c

all:
	gcc src/main.c ${OBJECTS} -o programaTrab.exe -Wall
	
run:
	./programaTrab.exe

clean:
	rm ./*bin ./*csv programaTrab.exe