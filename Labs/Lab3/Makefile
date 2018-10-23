CC = gcc
CFLAGS = -g -Wall -Wshadow -lpthread

smokers : smokers.c
	$(CC) smokers.c -o smokers $(CFLAGS)

test : smokers
	./smokers

clean :
	rm smokers
