CC = gcc
CFLAGS = -g -Wall -Wshadow

assignment2 : assignment2.c
	$(CC) assignment2.c -o assignment2 $(CFLAGS)
SJFMC : SJFMC.c
	$(CC) SJFMC.c -o SJFMC $(CFLAGS)
MCSJF : MCSJF.c
	$(CC) MCSJF.c -o MCSJF $(CFLAGS)
test1 : assignment2 test1.txt
	./assignment2 1 < test1.txt
test2 : assignment2 test2.txt
	./assignment2 2 < test2.txt
test3 : assignment2 test3.txt
	./assignment2 3 < test3.txt
test4 : assignment2 test4.txt
	./assignment2 4 < test4.txt
test5 : assignment2 test5.txt
	./assignment2 10 < test5.txt
test6 : assignment2 test6.txt
	./assignment2 5 < test6.txt
test7 : assignment2 test7.txt
	./assignment2 10 < test7.txt
test8 : assignment2 test8.txt
	./assignment2 4 < test8.txt
test1VAL : assignment2 test1.txt
	valgrind ./assignment2 < test1.txt
test2VAL : assignment2 test2.txt
	valgrind ./assignment2 < test2.txt
test3VAL : assignment2 test3.txt
	valgrind ./assignment2 < test3.txt
test4VAL : assignment2 test4.txt
	valgrind ./assignment2 < test4.txt
test5VAL : assignment2 test5.txt
	valgrind ./assignment2 < test5.txt
test6VAL : assignment2 test6.txt
	valgrind ./assignment2 < test6.txt
test7VAL : assignment2 test7txt
	valgrind ./assignment2 < test7.txt
test8VAL : assignment2 test8txt
	valgrind ./assignment2 < test8.txt
testMC1 : MCSJF test1.txt
	./MCSJF 1 < test1.txt
testMC2 : MCSJF test2.txt
	./MCSJF 2 < test2.txt
testMC3 : MCSJF test3.txt
	./MCSJF 3 < test3.txt
