prog: main.o tempsensor.o
	gcc -o cserver main.o tempsensor.o -lpthread

main.o: main.c
	gcc -c main.c

tempsensor.o: tempsensor.c tempsensor.h
	gcc -c tempsensor.c

clean:

