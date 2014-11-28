all:
	gcc -o any-ticker any-ticker.c && ./any-ticker
bk:
	git commit ; git push origin master

