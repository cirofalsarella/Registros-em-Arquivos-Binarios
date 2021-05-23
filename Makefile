OBJECTS = csvToBinary.c dataModel.c operations.c printer.c stringTable.c readBin.c

all:
	gcc main.c ${OBJECTS} -o programaTrab -Wall
	
run:
	./programaTrab

caso1:
	./programaTrab < Teste/1.in
	
caso2:
	./programaTrab < Teste/2.in
	
caso3:
	./programaTrab < Teste/3.in

caso4:
	./programaTrab < Teste/4.in

caso5:
	./programaTrab < Teste/5.in

caso6:
	./programaTrab < Teste/6.in

caso7:
	./programaTrab < Teste/7.in

caso8:
	./programaTrab < Teste/8.in

caso9:
	.programaTrab < Teste/9.in

caso10:
	.programaTrab < Teste/10.in
	
caso11:
	.programaTrab < Teste/11.in
	
caso12:
	.programaTrab < Teste/12.in
	
caso13:
	.programaTrab < Teste/13.in
	
caso14:
	.programaTrab < Teste/14.in
	
caso15:
	.programaTrab < Teste/15.in
	
caso16:
	.programaTrab < Teste/16.in