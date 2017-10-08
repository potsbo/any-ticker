CXX=g++
CXXFLAGS=-std=c++11

main: main.o any-ticker.o typeset.o set_parameters.o file_manage.o file_convert.o rleto106.o life_object.o ticker_message.o coordinate.o
	$(CXX) -o life main.o any-ticker.o typeset.o set_parameters.o file_manage.o file_convert.o rleto106.o life_object.o ticker_message.o coordinate.o
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
