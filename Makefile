OBJECTS = src/bTree/*.c src/core/*.c src/dataModel/*.c src/fileIO/*.c
MAIN = src/main.c
TEST_MAIN = test/testsMain.c

all:
	gcc ${MAIN} ${OBJECTS} -o programaTrab -Wall

run:
	./programaTrab

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./programaTrab

gdb:
	gdb programaTrab

# Windows-specific
c:
	gcc -ggdb3 ${MAIN} ${OBJECTS} -o programaTrab.exe -Wall

d:
	gcc -g ${MAIN} ${OBJECTS} -o programaTrab.exe -Wall
	gdb programaTrab.exe

r:
	gcc ${MAIN} ${OBJECTS} -o programaTrab.exe -Wall
	programaTrab.exe

TEST:
	make r < test_cases/in/$(CASE).in > test_cases/out/$(CASE).out

clean:
	rm ./*bin ./*csv programaTrab.exe