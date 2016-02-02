#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashTable.h"
#include "getWord.h"
#include "crc64.h"

wordPair* array = NULL;
int pos =0;
int printFlag=-1;

void print(wordPair* array){
	int nbrLine =0;
	if(printFlag != -1)
		nbrLine = printFlag;
	else
		nbrLine = pos;

	for(int i = 0; i < nbrLine; i++){
		if(nbrLine <= pos){
			printf("%10d %s\n",*((int*)array[i].val), array[i].key);
		}
		else{
		printf("Number of line resquested to print is greather than array size\n");
			break;
		}
	}
}

void makeArray(char* k, void* v){
	if(k != NULL && v != NULL){
		array[pos].key = k;
		array[pos].val = v;
		pos++;
	}
}

void freeWordPair(char*  k, void* v){
	free(k);
	free(v);
}

int cmpfunc(const void* pair1, const void* pair2){
	wordPair*  arg1 = (wordPair*)pair1; 
	wordPair*  arg2 = (wordPair*)pair2;
	int x = *((int*)(arg1->val));
	int y = *((int*)(arg2->val));
   
	if(x < y) return 1;
	else if(x > y) return -1;
	else return 0;	
}
void readWordPair(ht* hash_table, char* fname){
	FILE* fp;
	fp = fopen(fname, "r");
	char* word;
	char* prev;
	char* curr;
	char* pair;
  	int flag = 1;
	int nbrWord =0; // this variable is to check if there is at least two word

	while((word = getNextWord(fp))!= NULL){

   		if(flag == 1){
     	 	prev = (char*)malloc(sizeof(char)*(strlen(word)+1));
       	 	strcpy(prev, word);
     	 	flag++;
			nbrWord++;
    	}
		else if(flag != 1){
     	 	curr = (char*)malloc(sizeof(char)*(strlen(word)+1));
     		strcpy(curr, word);

			pair = (char*)malloc(sizeof(char)*(strlen(prev) + strlen(curr) + 2));
			strcpy(pair, prev);
			strcat(pair, "-");
			strcat(pair, curr);

			int* val = NULL;
			val  =(int*) htSearch(hash_table, pair);

			if(val != NULL){
				(*val)++;
				free(pair);
			}
			else if(val == NULL){
				val = (int*)malloc(sizeof(int));
				*val = 1;
				htInsert(hash_table, pair, val);
			}
			free(prev);
       		prev = (char*)malloc(sizeof(char)*(strlen(curr)+1));
        	strcpy(prev, curr);
			free(curr);
			nbrWord++;	
    	}  
		free(word);	
  	}
	free(prev);
	fclose(fp);
	if(nbrWord  <= 1){
		printf("No word pair found within the given file!!\n");
		printf("-.-.-.-.-.-.-.-.-.--.-.-.-.-.-.-.-.-.-.-.-.-.\n");
	}
	return;
}

int main(int argc, char** argv){
	ht* hash_table = htCreat(123);

	
	if(argc >= 2){

		char* str = argv[1];
		if(strchr(str, '-')){
			char* flag = (char*) malloc(sizeof(char) * strlen(str));
			strcpy(flag,&str[1]);
			printFlag = atoi(flag);
			free(flag);
			printf("Number of line to print: %d\n", printFlag);
			if(argc == 2){
				printf("!! No file path/name provided !!\n");
				return 0;
			}	
			for(int i=2; i<argc; i++){
				readWordPair(hash_table, argv[i]);
			}
		}
		else{
			for(int i=1; i<argc; i++){
				readWordPair(hash_table, argv[i]);
			}
		}
	}
	else{
		printf("too few argument !!! ./run -(integer) filename filename ...\n");
	}

	array = (wordPair*) malloc(sizeof(wordPair) * hash_table->numberEntries);
	for(int i=0; i<hash_table->numberEntries; i++){
		array[i].key = NULL;
		array[i].val = NULL;
		array[i].next = NULL;
	}
	htApply(hash_table, makeArray);
	printf("size: %d\n", hash_table->size);
	printf("number of entries: %d\n", hash_table->numberEntries);
	printf("---------------------------------------\n\n");
	qsort(array, pos, sizeof(wordPair), cmpfunc);
	print(array);
	htDelete(hash_table, freeWordPair);
	free(array);

 return 0;
}
