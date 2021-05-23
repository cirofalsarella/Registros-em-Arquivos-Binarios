OBJECTS = csvToBinary.c dataModel.c operations.c printer.c stringTable.c readBin.c funcao-fornecida.c

all:
	gcc main.c ${OBJECTS} -o programaTrab.exe -Wall
	
run:
	./programaTrab.exe

clean:
	rm ./*bin ./*csv programaTrab.exe