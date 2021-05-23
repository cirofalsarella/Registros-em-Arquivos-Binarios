OBJECTS = csvToBinary.c dataModel.c operations.c printer.c stringTable.c readBin.c

all:
	gcc main.c ${OBJECTS} -o programaTrab -Wall
	
run:
	./programaTrab

clean:
	rm ./*bin ./*csv programaTrab