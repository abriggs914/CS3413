CC = gcc
CFLAGS = -g -Wall -Wshadow

compile : profile-example.c valgrind-example.c
	$(CC) profile-example.c -o profile $(CFLAGS) -pg
	$(CC) valgrind-example.c -o valgrind $(CFLAGS)

valg : compile
	valgrind ./valgrind

gprof : compile
	./profile
	gprof profile gmon.out > analysis.txt
