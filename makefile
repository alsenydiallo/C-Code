
CC = gcc 
FLAGS =  -g -std=c99 -Wall -pedantic

wordpairs: countWord.o hashTable.o crc64.o
		$(CC) $(FLAGS) -o wordpairs countWord.o hashTable.o crc64.o getWord.o

countWord.o: countWord.c getWord.c
		$(CC) -c $(FLAGS) countWord.c getWord.c

crc64.o: crc64.c crc64.h
		$(CC) -c $(FLAGS) crc64.c

hashTable.o: hashTable.c hashTable.h
		$(CC) -c $(FLAGS) hashTable.c

getWord.o: getWord.c getWord.h
		$(CC) -c $(FLAGS) getWord.c

# Used to clean up the solution
# it will remove all .o file and the execution file
clean:
	rm *.o wordpairs
