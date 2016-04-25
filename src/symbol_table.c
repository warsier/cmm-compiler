#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symbol_table.h"
#include "syntax_tree.h"
#include "common.h"

SymbolNode *HashTable[HASH_MASK];
SymbolStackNode *SymbolStackHead = NULL;

unsigned int hashSymbol(char *name)
{
	unsigned int val = 0, i;
	for (; *name; name++) {
		val = (val << 2) + *name;
		if (i = val & ~HASH_MASK)
			val = (val ^ (i >> 12)) & HASH_MASK;
	}
	return val;
}

void clearSymbolStack()
{
	while (SymbolStackHead != NULL) {
		SymbolStackNode *p = SymbolStackHead;
		SymbolStackHead = SymbolStackHead->next;
		while (p->SymbolHead != NULL) {
			SymbolNode *q = p->SymbolHead;
			p->SymbolHead = p->SymbolHead->Stacknext;
			free(q);
		}
		free(p);
	}
	 
}

void buildSymbolTable(TreeNode *p)
{
	
}


void procSymbolTable(TreeNode *p)
{
	clearSymbolStack();
	buildSymbolTable(p);
	printf("ohhhhhhh!%s\n", p->text);
}

