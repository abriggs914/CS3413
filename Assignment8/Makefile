CC = gcc
CFLAGS = -g -Wall -Wshadow

memSwap : memSwap.c
	$(CC) memSwap.c -o memSwap $(CFLAGS)

test1 : memSwap test1.txt
	./memSwap 10 -f < test1.txt

test2 : memSwap test1.txt
	./memSwap 10 -l < test1.txt

test3 : memSwap test2.txt
	./memSwap 2 -f < test2.txt

test4 : memSwap test3.txt
	./memSwap 3 -f < test3.txt

test5 : memSwap test4.txt
	./memSwap 2 -f < test4.txt

test1v : memSwap test1.txt
	valgrind ./memSwap 10 -f < test1.txt --leak-check=full

test2v : memSwap test1.txt
	valgrind ./memSwap 10 -l < test1.txt --leak-check=full

clean : memSwap
	rm memSwap
