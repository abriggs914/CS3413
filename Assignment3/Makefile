CC = gcc
CFLAGS = -g -Wall -Wshadow -lpthread

pthread_SJF : pthread_SJF.c
	$(CC) pthread_SJF.c -o pthread_SJF $(CFLAGS)

test1 : pthread_SJF test1.txt
	./pthread_SJF 1 < test1.txt

test2 : pthread_SJF test2.txt
	./pthread_SJF 2 < test2.txt

test3 : pthread_SJF test3.txt
	./pthread_SJF 3 < test3.txt

test4 : pthread_SJF test4.txt
	./pthread_SJF 4 < test4.txt

test5 : pthread_SJF test5.txt
	./pthread_SJF 10 < test5.txt

test6 : pthread_SJF test6.txt
	./pthread_SJF 5 < test6.txt

test7 : pthread_SJF test7.txt
	./pthread_SJF 10 < test7.txt

test8 : pthread_SJF test8.txt
	./pthread_SJF 4 < test8.txtt

test1VAL : pthread_SJF test1.txt
	valgrind ./pthread_SJF 2 < test2.txt

test2VAL : pthread_SJF test2.txt
	valgrind ./pthread_SJF 2 < test2.txt

test3VAL : pthread_SJF test3.txt
	valgrind ./pthread_SJF 4 < test3.txt

test4VAL : pthread_SJF test4.txt
	valgrind ./pthread_SJF 5 < test4.txt

test5VAL : pthread_SJF test5.txt
	valgrind ./pthread_SJF 4 < test5.txt

test6VAL : pthread_SJF test6.txt
	valgrind ./pthread_SJF 6 < test6.txt

test7VAL : pthread_SJF test7txt
	valgrind ./pthread_SJF 2 < test7.txt

test8VAL : pthread_SJF test8txt
	valgrind ./pthread_SJF < test8.txt
