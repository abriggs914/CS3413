CC = gcc
CFLAGS = -g -Wall -Wshadow -lpthread

bridge : bridge.c
	$(CC) bridge.c -o bridge $(CFLAGS)

test1 : bridge test1.txt	#Original test
	./bridge 5 < test1.txt

test2 : bridge test1.txt	#testing speedLimit argument
		./bridge 3 < test1.txt

test3 : bridge test2.txt	#testing bridge openings
		./bridge 5 < test2.txt

test4 : bridge test3.txt	#testing <3 cars per queue
	./bridge 5 < test3.txt

test5 : bridge test1.txt #testing speedLimit argument
	./bridge 1 < test1.txt

clean :
	rm bridge
