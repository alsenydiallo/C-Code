

typedef struct _wordPair {
	char *key;
	void *val;
	struct _wordPair *next;
} wordPair;

typedef struct {
	int size;
	int numberEntries;
	wordPair** wordPairTable;
} ht;

ht* htCreat(int size);
void* htSearch(ht* hash_table, char* key);
int htInsert(ht* hash_table, char* key, void* val);
void htDelete(ht* hash_table, void(*f)(char* key, void*));
void htApply(ht* hash_table, void(*f)(char* key, void*));
void destroyHashTable(ht* hash_table);

