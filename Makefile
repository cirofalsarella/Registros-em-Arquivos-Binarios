OBJECTS = csvToBinary.c dataModel.c operations.c printer.c stringTable.c

all:
	gcc main.c ${OBJECTS} -o programaTrab 
	
run:
	./programaTrab

caso1:
	./programaTrab < Teste/caso1
	
caso2:
	./programaTrab < Teste/caso2
	