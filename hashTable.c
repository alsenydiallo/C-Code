#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hashTable.h"
#include "getWord.h"
#include "crc64.h"

ht* htCreat(int size){
	ht* hash_table = (ht*) malloc(sizeof(ht));
	hash_table->size = size;
	hash_table->numberEntries = 0;

	hash_table->wordPairTable = (wordPair**) malloc(sizeof(wordPair*) * size);
	for(int i=0; i < size; i++){
		hash_table->wordPairTable[i] = NULL;
	}
	return hash_table;
}

void htGrow(ht* oldHash_table, int size){

	wordPair** oldWordPairTable = oldHash_table->wordPairTable;
	wordPair** newWordPairTable = (wordPair**)malloc(sizeof(wordPair*) * size);
	for(int i=0; i < size; i++){
		newWordPairTable[i] = NULL;
	}

	for(int i = 0; i < oldHash_table->size; i++){
		if(oldWordPairTable[i] != NULL){
			while(oldWordPairTable[i] != NULL){
				wordPair *temp = oldWordPairTable[i];
				oldWordPairTable[i] = temp->next;
				temp->next = NULL;
				int newIndex = crc64(temp->key) % size;
				if(newWordPairTable[newIndex] == NULL){
					newWordPairTable[newIndex] = temp;	
				}
				else{
					temp->next = newWordPairTable[newIndex];
					newWordPairTable[newIndex] = temp;
				}
				temp = oldWordPairTable[i];
			}
		}
		free(oldWordPairTable[i]);
	}

	free(oldWordPairTable);
	oldHash_table->wordPairTable = newWordPairTable;
	oldHash_table->size = size;
}

int htInsert(ht* hash_table, char* k, void* v){
		int hashIndex = crc64(k) % (hash_table->size);
		float x = ((hash_table->numberEntries)/ ((double)(hash_table->size)));

		wordPair *_wordpair = (wordPair*) malloc(sizeof(wordPair));
		_wordpair->key = k;
		_wordpair->val = v;
		_wordpair->next = NULL;

		if(x >= 0.75){
			htGrow(hash_table, 3 * hash_table->size);
		}

		if(hash_table->wordPairTable[hashIndex] == NULL){
			hash_table->wordPairTable[hashIndex] = _wordpair;
			hash_table->numberEntries++;
			return 1;
		}
		else{
			_wordpair->next = hash_table->wordPairTable[hashIndex];
			hash_table->wordPairTable[hashIndex] = _wordpair;
			hash_table->numberEntries++;
			return 1;
		}
	return 0;
}

void* htSearch(ht* hash_table, char* key){

		int hashIndex = crc64(key)%(hash_table->size);
		wordPair** wordpairs = hash_table->wordPairTable;

		if(wordpairs[hashIndex] != NULL){
			wordPair* temp = wordpairs[hashIndex];
			while(temp != NULL){
				if(strcmp(key, temp->key) == 0){
					return temp->val;
				}
				else
					temp = temp->next;
			}
		}
		return NULL;
}

void htApply(ht* hash_table, void(*f)(char *k, void* v)){
		int size = hash_table->size;
		wordPair** wordpairs= hash_table->wordPairTable;

		for(int i=0; i<size; i++){
			if(wordpairs[i] != NULL){
				f(wordpairs[i]->key, wordpairs[i]->val);
		
				if(wordpairs[i]->next != NULL){
					wordPair* curr = wordpairs[i];
					while(curr->next != NULL){
						curr = curr->next;
						f(curr->key, curr->val);
					}
				}
		
			}
		}	
}

void htDelete(ht* hash_table, void(*f)(char *k, void* v)){
		int size = hash_table->size;
		for(int i =0; i<size; i++){
			wordPair* temp = hash_table->wordPairTable[i];
			wordPair* prev;
			if(temp != NULL){
				while(temp != NULL){
					f(temp->key, temp->val);
					prev = temp;
					temp = temp->next;
					free(prev);
				}
			}
		//	else
		//		free(temp);
		}
	free(hash_table->wordPairTable);
	free(hash_table);	
}

void destroyHashTable(ht* hash_table){
	int size = hash_table->size;
	wordPair** wordpairs = hash_table->wordPairTable;

	for(int i=0; i < size; i++){
		if(wordpairs[i] != NULL){
			free(wordpairs[i]->key);
			free(wordpairs[i]->val);

			if(wordpairs[i]->next != NULL){
				wordPair* curr = wordpairs[i];
				while(curr->next != NULL)
				{
					curr = curr->next;
					wordPair* temp = curr;
					free(curr->key);
					free(curr->val);
					free(temp);
				}
			}
		}
		else
			free(wordpairs[i]);
	}
	free(hash_table);
}
