CC = gcc
CFLAGS = -g -Wall -Wshadow

# test1.txt - all allocated, P3 not ended
# test2.txt - all allocated, all freed
# test3.txt - same as test2.txt, but P1 is 2x bigger, used for bf and wf tests
# test4.txt - same as test3.txt, but P4 & P5 get significantly smaller
# test5.txt - all allocated, all freed, random spaces created
# test6.txt - all but P1 & P8 are freed, 10 Processes, random terminations

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

clean :
	rm mem_alloc
