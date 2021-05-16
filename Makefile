all:
	gcc -o programaTrab *.c
	
run:
	./programaTrab

# 	BASE = stringTable.c dataModel.c csvToBinary.c
# 
# all:
# 	gcc -g -ggdb3 -Wall $(BASE) main.c -o main
# run:
# 	make all
# 	gdb main.exe