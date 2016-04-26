#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symbol_table.h"
#include "syntax_tree.h"
#include "common.h"

SymbolNode *HashTable[HASH_MASK];
SymbolStackNode *SymbolStackHead = NULL;

unsigned int hashSymbol(const char *name)
{
	unsigned int val = 0, i;
	for (; *name; name++) {
		val = (val << 2) + *name;
		if (i = val & ~HASH_MASK)
			val = (val ^ (i >> 12)) & HASH_MASK;
	}
	return val;
}

// Given the symbol name, return an empty SymbolNode generated at the correct position.
SymbolNode *pushinSymbol(const char *name)
{
	SymbolNode *hashslot = HashTable[hashSymbol(name)]; // generate the hash slot
	SymbolNode *stackslot = SymbolStackHead->SymbolHead;
	SymbolNode *newnode = (SymbolNode *) malloc(sizeof(SymbolNode));
	
	if (hashslot == NULL) {
		hashslot = newnode;
		newnode->HashNext = NULL;
	}
	else {
		newnode->HashNext = hashslot;
		hashslot = newnode;
	}
	
	if (stackslot == NULL) {
		stackslot = newnode;
		newnode->StackNext = NULL;
	}
	else {
		newnode->StackNext = stackslot;
		stackslot = newnode;
	}
	
	newnode->toHead = SymbolStackHead;
	
	// remember that we did not modify the original pointer
	HashTable[hashSymbol(name)] = hashslot;
	SymbolStackHead->SymbolHead = stackslot;
	
	return newnode;
}

void clearSymbolStack()
{
	while (SymbolStackHead != NULL) {
		SymbolStackNode *p = SymbolStackHead;
		SymbolStackHead = SymbolStackHead->next;
		while (p->SymbolHead != NULL) {
			SymbolNode *q = p->SymbolHead;
			p->SymbolHead = p->SymbolHead->StackNext;
			free(q);
		}
		free(p);
	}
	int i;
	for (i = 0; i < HASH_MASK; i++)
		HashTable[i] = NULL;
	 
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
		SymbolStackNode *newstacknode = (SymbolStackNode *) malloc(sizeof(SymbolStackNode));
		newstacknode->next = SymbolStackHead;
		SymbolStackHead = newstacknode;
		
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
	SymbolStackHead = (SymbolStackNode *) malloc(sizeof(SymbolStackNode)); // global symbol table
	SymbolStackHead->next = NULL;
	SymbolStackHead->SymbolHead = NULL;
	buildSymbolTable(p);
	printf("ohhhhhhh!%s\n", p->text);
	clearSymbolStack();
}

