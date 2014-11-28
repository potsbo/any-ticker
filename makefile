all:
	gcc -o any-ticker any-ticker.c && ./any-ticker
bk:
	git commit ; git push origin master
e:
	${EDITOR} any-ticker.c
show:
	open any-ticker.life -a Golly

