CC = gcc
CFLAGS = -g -Wall -Wshadow

compile : profile-example.c valgrind-example.c mem_alloc.c
	$(CC) profile-example.c -o profile $(CFLAGS) -pg
	$(CC) valgrind-example.c -o valgrind $(CFLAGS)
	$(CC) mem_alloc.c -o mem_alloc $(CFLAGS) -pg

valg : compile
	valgrind ./valgrind

gprof : compile
	./profile
	gprof profile gmon.out > analysis.txt

test1 : mem_alloc test1.txt #first fit and not freeing a process
	./mem_alloc -s 1000 -f < test1.txt

test2 : mem_alloc test2.txt#first fit
	./mem_alloc -s 250 -f < test2.txt

test3 : mem_alloc test2.txt #best fit
	./mem_alloc -s 2000 -b < test2.txt

test4 : mem_alloc test3.txt #best fit
	./mem_alloc -s 2000 -b < test3.txt

test5 : mem_alloc test4.txt #best fit forces P5 into end space of memory instead of beginning
	./mem_alloc -s 2000 -b < test4.txt

test6 : mem_alloc test4.txt #worst fit forces P5 into end space of memory instead of beginning
	./mem_alloc -s 2000 -w < test4.txt

test7 : mem_alloc test5.txt #first fit
	./mem_alloc -s 2000 -f < test5.txt > test7Out.txt

test8 : mem_alloc test5.txt #best fit
	./mem_alloc -s 2000 -b < test5.txt > test8Out.txt

test9 : mem_alloc test5.txt #worst fit
	./mem_alloc -s 2000 -w < test5.txt > test9Out.txt

test10 : mem_alloc test5.txt #first fit
	./mem_alloc -s 2000 -f < test6.txt

test11 : mem_alloc test5.txt #best fit
	./mem_alloc -s 2000 -b < test6.txt

test12 : mem_alloc test5.txt #worst fit
	./mem_alloc -s 2000 -w < test6.txt

test13 : test8 test9 #this test shows the difference between bf and wf
	diff test8Out.txt test9Out.txt

gprof_MEM : mem_alloc test1
	gprof mem_alloc gmon.out > analysis.txt

valgrind_MEM : mem_alloc test1.txt
	valgrind ./mem_alloc -s 1000 -f < test1.txt

clean :
	rm mem_alloc profile valgrind
