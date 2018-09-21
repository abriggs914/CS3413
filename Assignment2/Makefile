CC = gcc
CFLAGS = -g -Wall -Wshadow

SJFMC : SJFMC.c
	$(CC) SJFMC.c -o SJFMC $(CFLAGS)
MCSJF : MCSJF.c
	$(CC) MCSJF.c -o MCSJF $(CFLAGS)
test1 : SJFMC test1.txt
	./SJFMC 1 < test1.txt
test2 : SJFMC test2.txt
	./SJFMC 2 < test2.txt
test3 : SJFMC test3.txt
	./SJFMC 3 < test3.txt
test4 : SJFMC test4.txt
	./SJFMC 4 < test4.txt
test5 : SJFMC test5.txt
	./SJFMC 1 < test5.txt
test6 : SJFMC test6.txt
	./SJFMC 5 < test6.txt
test7 : SJFMC test7.txt
	./SJFMC 10 < test7.txt
test8 : SJFMC test8.txt
	./SJFMC 4 < test8.txt
test1VAL : SJFMC test1.txt
	valgrind ./SJFMC < test1.txt
test2VAL : SJFMC test2.txt
	valgrind ./SJFMC < test2.txt
test3VAL : SJFMC test3.txt
	valgrind ./SJFMC < test3.txt
test4VAL : SJFMC test4.txt
	valgrind ./SJFMC < test4.txt
test5VAL : SJFMC test5.txt
	valgrind ./SJFMC < test5.txt
test6VAL : SJFMC test6.txt
	valgrind ./SJFMC < test6.txt
test7VAL : SJFMC test7txt
	valgrind ./SJFMC < test7.txt
test8VAL : SJFMC test8txt
	valgrind ./SJFMC < test8.txt
testMC1 : MCSJF test1.txt
	./MCSJF 1 < test1.txt
testMC2 : MCSJF test2.txt
	./MCSJF 2 < test2.txt
testMC3 : MCSJF test3.txt
	./MCSJF 3 < test3.txt
