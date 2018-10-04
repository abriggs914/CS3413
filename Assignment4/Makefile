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

