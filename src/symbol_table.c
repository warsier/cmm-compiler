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

void procExtDef(TreeNode *p)
{
	if (STREQ(p->children[1]->symbol, "ExtDecList")) {
		printf("ExtDecList\n");
	}
	else if (STREQ(p->children[1]->symbol, "FunDec")) {
		printf("FunDec\n");
		buildSymbolTable(p->children[2]);
	}
}

void buildSymbolTable(TreeNode *p)
{
	if (STREQ(p->symbol, "ExtDef")) {
		// if it is a global definition (can be a function or a variable)
		printf("ExtDef\n");
		procExtDef(p);
	}
	else if (STREQ(p->symbol, "Def")) {
		// if it is a local variable definition
		printf("Def\n");
	}
	else if (STREQ(p->symbol, "Exp")) {
		// if it is a variable use or function call
		printf("Exp\n");
	}
	else if (STREQ(p->symbol, "LC")) {
		printf("LC\n");
	}
	else if (STREQ(p->symbol, "RC")) {
		printf("RC\n");
	}
	else {
		int i;
		for (i = 0; i < p->arity; i++)
			buildSymbolTable(p->children[i]);
	}
}


void procSymbolTable(TreeNode *p)
{
	clearSymbolStack();
	buildSymbolTable(p);
	printf("ohhhhhhh!%s\n", p->text);
	clearSymbolStack();
}

