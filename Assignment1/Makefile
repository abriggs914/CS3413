CC = gcc
CFLAGS = -g -Wall -Wshadow

SJF : SJF.c
	$(CC) SJF.c -o SJF $(CFLAGS)
test1 : SJF test1.txt
	./SJF < test1.txt
test2 : SJF test2.txt
	./SJF < test2.txt
test3 : SJF test3.txt
	./SJF < test3.txt
test4 : SJF test4.txt
	./SJF < test4.txt
test1VAL : SJF test1.txt
	valgrind ./SJF < test1.txt
test2VAL : SJF test2.txt
	valgrind ./SJF < test2.txt
test3VAL : SJF test3.txt
	valgrind ./SJF < test3.txt
test4VAL : SJF test4.txt
	valgrind ./SJF < test4.txt
