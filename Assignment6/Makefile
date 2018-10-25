CC = gcc
CFLAGS = -g -Wall -Wshadow

mem_alloc : mem_alloc.c
	$(CC) mem_alloc.c -o mem_alloc

test1 : mem_alloc
	./mem_alloc -s 1000 -f < test1.txt
