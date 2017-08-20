CC=g++
CFLAGS=

main: main.o any-ticker.o typeset.o set_parameters.o file_manage.o file_convert.o rleto106.o life_object.o
	$(CC) -o life main.o any-ticker.o typeset.o set_parameters.o file_manage.o file_convert.o rleto106.o life_object.o
run: main
	./life
bk:
	git commit ; git push origin master
show:
	open any-ticker.life -a Golly
test: main
	./test.sh
clean:
	rm -f any-ticker; rm -f *.o; rm -f *.life; rm -f life
