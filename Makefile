all:	backup test

backup:
	gcc -g -o backup backup.c
test:
	gcc -g -o test test.c
clean:
	rm backup test
