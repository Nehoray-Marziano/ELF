all: good


good: task2.o
	gcc -g -m32 -fno-pie -Wall -o good task2.o


task2.o: task2.c
	gcc -m32 -fno-pie -Wall -ansi -c -o task2.o task2.c

.PHONY:clean
	
clean:
	rm -f *o good
