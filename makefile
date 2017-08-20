CC=g++
CFLAGS=

main: main.o any-ticker.o typeset.o set_parameters.o file_manage.o file_convert.o rleto106.o
	$(CC) -o life main.o any-ticker.o typeset.o set_parameters.o file_manage.o file_convert.o rleto106.o
run: main
	./life
bk:
	git commit ; git push origin master
show:
	open any-ticker.life -a Golly
test:
	test/run
clean:
	rm -f any-ticker; rm -f *.o; rm -f *.life; rm -f life
