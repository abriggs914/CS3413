CC = gcc
CFLAGS = -g -Wall -Wshadow

addressConvert : addressConvert.c
	$(CC) addressConvert.c -o addressConvert $(CFLAGS)

test1 : addressConvert test1.txt
	./addressConvert 10 < test1.txt

test2 : addressConvert test2.txt
	./addressConvert 2 < test2.txt

test3 : addressConvert test1.txt
	./addressConvert 2 < test1.txt

test4 : addressConvert test4.txt
	./addressConvert 2 < test4.txt

test5 : addressConvert test5.txt
	./addressConvert 1 < test5.txt

test6 : addressConvert test4.txt
	./addressConvert 7 < test4.txt

test7 : addressConvert test5.txt
	./addressConvert 1048576 < test5.txt

basht : addressConvert
	./addressConvert 10

clean :
	rm addressConvert
