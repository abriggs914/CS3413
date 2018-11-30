(CC) = gcc
(CFLAGS) = -g -Wall -Wshadow -lm

inT : inputTest
	$(CC) inputTest.c -o inT $(CFLAGS)

linkedDisk : linkedDisk.c
	$(CC) linkedDisk.c -o linkedDisk $(CFLAGS)

# test1 : linkedDisk test1.txt
# 	./linkedDisk F 53 < test1.txt

# test2 : linkedDisk test2.txt
# 	./linkedDisk T 53 < test2.txt

# test3 : linkedDisk test3.txt
# 	./linkedDisk T 990 < test3.txt

# test4 : linkedDisk test3.txt
# 	./linkedDisk C 999 < test3.txt

# test5 : linkedDisk test3.txt
# 	./linkedDisk F 53 < test3.txt

# test6 : linkedDisk test3.txt
# 	./linkedDisk T 53 < test3.txt

# test6 : linkedDisk test3.txt
# 	./linkedDisk C 53 < test3.txt

# test6 : linkedDisk test3.txt
	./linkedDisk L 53 < test3.txt

clean : 
	rm linkedDisk