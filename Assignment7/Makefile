CC = gcc
CFLAGS = -g -Wall -Wshadow

addressConvert : addressConvert.c
	$(CC) addressConvert.c -o addressConvert $(CFLAGS)

test1 : addressConvert test1.txt
	./addressConvert 10 < test1.txt > test1OUT.txt
	diff test1OUT.txt test1EXPEC.txt

basht : addressConvert
	./addressConvert 10
