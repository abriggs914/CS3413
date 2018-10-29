CC = gcc
CFLAGS = -g -Wall -Wshadow

mem_alloc : mem_alloc.c
	$(CC) mem_alloc.c -o mem_alloc $(CFLAGS)

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

clean :
	rm mem_alloc
