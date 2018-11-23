(CC) = gcc
(CFLAGS) = -g -Wall -Wshadow -lm

memDisk : memDisk.c
	$(CC) memDisk.c -o memDisk $(CFLAGS)

test1 : memDisk test1.txt
	./memDisk T 53 < test1.txt

clean : 
	rm memDisk