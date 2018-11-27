(CC) = gcc
(CFLAGS) = -g -Wall -Wshadow -lm

memDisk : memDisk.c
	$(CC) memDisk.c -o memDisk $(CFLAGS)

test1 : memDisk test1.txt
	./memDisk F 53 < test1.txt

test2 : memDisk test2.txt
	./memDisk T 53 < test2.txt

test3 : memDisk test3.txt
	./memDisk T 53 < test3.txt

test4 : memDisk test3.txt
	./memDisk C 999 < test3.txt

test5 : memDisk test3.txt
	./memDisk L 53 < test3.txt

clean : 
	rm memDisk