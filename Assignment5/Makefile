(CC) = gcc
(CFLAGS) = -Wall -lpthread -Wshadow -g

bridge : bridge.c
	$(CC) bridge.c -o bridge $(CFLAGS)

test1 : bridge test1.txt
	./bridge 5 < test1.txt

test2 : bridge test1.txt
		./bridge 3 < test1.txt
