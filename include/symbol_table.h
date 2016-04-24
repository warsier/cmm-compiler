#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#define HASH_MASK (0x3fff) // the size of hash table

typedef struct TableNode
{
	
} TableNode;

TableNode *HashTable[HASH_MASK];
TableNode *TableStack;

unsigned int hash(char *name);

#endif