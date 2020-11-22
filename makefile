mosaico: lib_mosaico.o mosaico.o
	gcc -g -Wall mosaico.o lib_mosaico.o -lm -o mosaico

mosaico.o: lib_mosaico.h mosaico.c
	gcc -Wall -g -c mosaico.c

lib_mosaico.o: lib_mosaico.c lib_mosaico.h
	gcc -Wall -g -c lib_mosaico.c
