CC = gcc
CFLAGS = -g -Wall -Wshadow

memSwap : memSwap.c
	$(CC) memSwap.c -o memSwap

test1 : memSwap test1.txt
	./memSwap 10 -f < test1.txt

clean : memSwap
	rm memSwap