CC = gcc
CFLAGS = -g -Wall -Wshadow

multi_core_SJF : multi_core_SJF.c
	$(CC) multi_core_SJF.c -o multi_core_SJF $(CFLAGS)
assignment2 : assignment2.c
	$(CC) assignment2.c -o assignment2 $(CFLAGS)
SJFMC : SJFMC.c
	$(CC) SJFMC.c -o SJFMC $(CFLAGS)
MCSJF : MCSJF.c
	$(CC) MCSJF.c -o MCSJF $(CFLAGS)
test1 : multi_core_SJF test1.txt
	./multi_core_SJF 1 < test1.txt
test2 : multi_core_SJF test2.txt
	./multi_core_SJF 2 < test2.txt
test3 : multi_core_SJF test3.txt
	./multi_core_SJF 3 < test3.txt
test4 : multi_core_SJF test4.txt
	./multi_core_SJF 4 < test4.txt
test5 : multi_core_SJF test5.txt
	./multi_core_SJF 10 < test5.txt
test6 : multi_core_SJF test6.txt
	./multi_core_SJF 5 < test6.txt
test7 : multi_core_SJF test7.txt
	./multi_core_SJF 10 < test7.txt
test8 : multi_core_SJF test8.txt
	./multi_core_SJF 4 < test8.txt
test1VAL : multi_core_SJF test1.txt
	valgrind ./multi_core_SJF < test1.txt
test2VAL : multi_core_SJF test2.txt
	valgrind ./multi_core_SJF < test2.txt
test3VAL : multi_core_SJF test3.txt
	valgrind ./multi_core_SJF < test3.txt
test4VAL : multi_core_SJF test4.txt
	valgrind ./multi_core_SJF < test4.txt
test5VAL : multi_core_SJF test5.txt
	valgrind ./multi_core_SJF < test5.txt
test6VAL : multi_core_SJF test6.txt
	valgrind ./multi_core_SJF < test6.txt
test7VAL : multi_core_SJF test7txt
	valgrind ./multi_core_SJF < test7.txt
test8VAL : multi_core_SJF test8txt
	valgrind ./multi_core_SJF < test8.txt
testMC1 : MCSJF test1.txt
	./MCSJF 1 < test1.txt
testMC2 : MCSJF test2.txt
	./MCSJF 2 < test2.txt
testMC3 : MCSJF test3.txt
	./MCSJF 3 < test3.txt
