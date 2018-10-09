CC = gcc
CFLAGS = -g -Wall -Wshadow -lpthread

sleepingTA : sleepingTA.c
	$(CC) sleepingTA.c -o sleepingTA $(CFLAGS)

test1 : sleepingTA
	./sleepingTA -c 5 -t 200 -n 12 
	
test2 : sleepingTA
	./sleepingTA -c 10 -t 500 -n 15 
	
test3 : sleepingTA
	./sleepingTA -c 8 -t 0 -n 12 

clean : 
	rm sleepingTA
