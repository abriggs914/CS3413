CC = gcc
CFLAGS = -g -Wall -Werror -lpthread

encode : encode.c
	$(CC) encode.c -o encode $(CFLAGS)
test1 : encode test1.txt
	./encode < test1.txt
test2 : encode test2.txt
	./encode < test2.txt
test3 : encode test3.txt
	./encode < test3.txt
