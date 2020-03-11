all: foobar

foobar: warmup.c rpthread.o
	gcc -g -o foobar warmup.c rpthread.o

rpthread.o: ../code/rpthread.h
	gcc -g -c ../code/rpthread.c
clean:
	rm -rf foobar *.o
