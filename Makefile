FLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -pedantic

all: main

main: main.o reseau.o utils.o
	gcc main.o reseau.o utils.o -o main

main.o: main.c reseau.h
	gcc -c main.c $(FLAGS)

reseau.o: reseau.c reseau.h
	gcc -c reseau.c $(FLAGS)

utils.o: utils.c
	gcc -c utils.c $(FLAGS)

clean:
	rm -f main.o reseau.o utils.o main