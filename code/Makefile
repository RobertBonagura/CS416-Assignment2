all: foobar

foobar: test.c rpthread.o
	gcc -g -o foobar test.c rpthread.o

rpthread.o: rpthread.h
	gcc -g -c rpthread.c
clean:
	rm -rf foobar *.o
