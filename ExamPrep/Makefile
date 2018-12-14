CC = gcc
CFLAGS = -g -lpthread -Wall -Wshadow

pipe : pipe.c
	$(CC) pipe.c -o pipe $(CFLAGS)

test1: pipe test1.txt
	./pipe < test1.txt
