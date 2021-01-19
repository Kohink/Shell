all:
	gcc -std=c -o shell *.c

clean:
	rm -f shell *.o core.* *.gch