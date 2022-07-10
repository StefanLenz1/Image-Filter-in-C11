CC=clang
CFLAGS= -std=c11 -Wall -g -w -Werror -O2

all: imagefilter

imagefilter.o: imagefilter.c filters.h
	$(CC) $(CLAGS) -c imagefilter.c

filters.o: filters.h filters.c bmp.h
	$(CC) $(CLAGS) -c filters.c

imagefilter: imagefilter.o filters.o
	$(CC) $(CFLAGS) -o imagefilter imagefilter.o filters.o -lm

clean:
	-rm *.o
	-rm imagefilter
	-rm output/*