all: task2


task2: task2.o
	gcc -g -m32 -Wall -o task2 task2.o


task2.o: task2.c
	gcc -m32 -g -Wall -ansi -c -o task2.o task2.c

clean:
	rm -f *o task2
