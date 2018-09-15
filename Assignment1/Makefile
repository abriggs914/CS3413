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
test5 : SJF test5.txt
	./SJF < test5.txt
test6 : SJF test6.txt
	./SJF < test6.txt
test7 : SJF test7.txt
	./SJF < test7.txt
test1VAL : SJF test1.txt
	valgrind ./SJF < test1.txt
test2VAL : SJF test2.txt
	valgrind ./SJF < test2.txt
test3VAL : SJF test3.txt
	valgrind ./SJF < test3.txt
test4VAL : SJF test4.txt
	valgrind ./SJF < test4.txt
test5VAL : SJF test5.txt
	valgrind ./SJF < test5.txt
test6VAL : SJF test6.txt
	valgrind ./SJF < test6.txt
test7VAL : SJF test67txt
	valgrind ./SJF < test7.txt
