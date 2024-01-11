all: main

main: main.o
	gcc -o main main.o

main.o: src/main.c
	gcc -c src/main.c

clean:
	rm -f main main.o
