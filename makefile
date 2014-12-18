CC=gcc
CFLAGS=-I.

all: any-ticker.h
	$(CC) -o any-ticker any-ticker.c $(CFLAGS) && ./any-ticker
bk:
	git commit ; git push origin master
e:
	${EDITOR} any-ticker.c
show:
	open any-ticker.life -a Golly

