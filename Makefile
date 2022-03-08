CC=gcc
CFLAGS=-I.

hinfosvc: hinfosvc.o 
	$(CC) -o hinfosvc hinfosvc.o
	rm -f *.o *~ 
