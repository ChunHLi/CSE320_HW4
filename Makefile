all:
	gcc -pthread -o part1 part1.c cse320_functions.c csapp.c
clean:
	rm -f part1
