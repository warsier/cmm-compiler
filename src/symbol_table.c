#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symbol_table.h"
#include "syntax_tree.h"
#include "common.h"

SymbolNode *HashTable[HASH_MASK];
SymbolStackNode *SymbolStackHead = NULL;

void symbolErrorMsg(char ErrorType, TreeNode *p)
{
	switch(ErrorType) {
	case '1': printf("Error type 1 at line %d: Undefined variable \"%s\".\n", p->lineno, p->text); break;
	case '2': printf("Error type 2 at line %d: Undefined function \"%s\".\n", p->lineno, p->text); break;
	case '3': printf("Error type 3 at line %d: Redefined variable \"%s\".\n", p->lineno, p->text); break;
	}
}

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

bool searchSymbol(const char *name)
{
	SymbolNode *temp = HashTable[hashSymbol(name)]; // generate the hash slot
	for (; temp != NULL; temp = temp->HashNext) {
		if (STREQ(temp->text, name))
			return true;
	}
	return false;
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
			SymbolNode *temp = p->SymbolHead;
			if (temp->isfunc && temp->FuncMsg.ArgNum > 0)
				free(temp->FuncMsg.ArgType);
			p->SymbolHead = p->SymbolHead->StackNext;
			free(temp);
		}
		free(p);
	}
	int i;
	for (i = 0; i < HASH_MASK; i++)
		HashTable[i] = NULL;
	 
}

// ExtDef -> Specifier ExtDecList SEMI (global variable definition)
//         | Specifier SEMI            (global structure definition)
//         | Specifier FunDec CompSt   (function definition)
// ExtDecList -> VarDec
//             | VarDec COMMA ExtDecList
// CompSt -> LC DefList StmtList RC (DefList and StmtList can be empty)
void procExtDef(TreeNode *p)
{
	if (STREQ(p->children[1]->symbol, "ExtDecList")) {
		Type nodetype = procSpecifier(p->children[0]);
		TreeNode *temp = p->children[1];
		while (temp->arity > 1) {
			procVarDec(nodetype, temp->children[0]);
			temp = temp->children[2];
		}
		procVarDec(nodetype, temp->children[0]);
	}
	else if (STREQ(p->children[1]->symbol, "FunDec")) {
		Type nodetype = procSpecifier(p->children[0]);
		procFunDec(nodetype, p->children[1]);
		int i;
		for (i = 1; i < p->children[2]->arity; i++)
			buildSymbolTable(p->children[2]->children[i]);
	}
	else {
		assert(0);
	}
}

// FunDec -> ID LP VarList RP
//         | ID LP RP
// VarList -> ParamDec COMMA VarList
//          | ParamDec
// ParamDec -> Specifier VarDec
void procFunDec(Type nodetype, TreeNode *p)
{
	SymbolNode *newnode = pushinSymbol(p->children[0]->text);
	strcpy(newnode->text, p->children[0]->text);
	newnode->isfunc = true, newnode->isdef = true;
	newnode->lineno = p->children[0]->lineno;
	newnode->FuncMsg.RetValType = nodetype;
	
	// push a new field into the stack
	SymbolStackNode *newstacknode = (SymbolStackNode *) malloc(sizeof(SymbolStackNode));
	newstacknode->next = SymbolStackHead;
	SymbolStackHead = newstacknode;
	SymbolStackHead->SymbolHead = NULL;
	
	if (p->arity == 4) {
		int cnt = 1;
		TreeNode *temp = p->children[2];
		while (temp->arity > 1) {
			cnt++;
			temp = temp->children[2];
		}
		newnode->FuncMsg.ArgNum = cnt;
		newnode->FuncMsg.ArgType = (Type *) malloc(sizeof(Type) * cnt);
		cnt = 0;
		temp = p->children[2];
		while (temp->arity > 1) {
			Type newnodetype = procSpecifier(temp->children[0]->children[0]);
			procVarDec(newnodetype, temp->children[0]->children[1]);
			newnode->FuncMsg.ArgType[cnt] = newnodetype;
			cnt++;
			temp = temp->children[2];
		}
		Type newnodetype = procSpecifier(temp->children[0]->children[0]);
		procVarDec(newnodetype, temp->children[0]->children[1]);
		newnode->FuncMsg.ArgType[cnt] = newnodetype;
		cnt++;
	}
	else if (p->arity == 3) {
		newnode->FuncMsg.ArgNum = 0;
		newnode->FuncMsg.ArgType = NULL; 
	}
	else {
		assert(0);
	}
}

// Def -> Specifier DecList SEMI
// DecList -> Dec
//          | Dec COMMA DecList
// Dec -> VarDec
//      | VarDec AssignOP Exp
void procDef(TreeNode *p)
{
	// Specifier can be TYPE or StructSpecifier
	Type nodetype = procSpecifier(p->children[0]);
	TreeNode *temp = p->children[1];
	while (temp->arity > 1) {
		procVarDec(nodetype, temp->children[0]->children[0]);
		temp = temp->children[2];
	}
	procVarDec(nodetype, temp->children[0]->children[0]);
}

// Specifier -> TYPE 
//            | StructSpecifier
Type procSpecifier(TreeNode *p)
{
	Type nodetype;
	if (STREQ(p->children[0]->symbol, "TYPE")) {
		nodetype.kind = BASIC;
		if (STREQ(p->children[0]->text, "INT")) nodetype.basic = B_INT;
		else nodetype.basic = B_FLOAT;
	}
	else {
		// implement structure later
		assert(0);
	}
	return nodetype;
}

// VarDec -> ID
//         | VarDec LB INT RB
void procVarDec(Type nodetype, TreeNode *p)
{
	if (p->arity == 1) {
		if (searchSymbol(p->children[0]->text)) {
			symbolErrorMsg('3', p->children[0]);
		}
		SymbolNode *newnode = pushinSymbol(p->children[0]->text);
		strcpy(newnode->text, p->children[0]->text);
		newnode->isfunc = false, newnode->isdef = true;
		newnode->lineno = p->children[0]->lineno;
		newnode->VarMsg = nodetype;
	}
	else {
		// implement array later
		assert(0);
	}
}

void procExp(TreeNode *p)
{
	int i;
	if (p->arity == 1 && STREQ(p->children[0]->symbol, "ID")) {
		if(!searchSymbol(p->children[0]->text))
			symbolErrorMsg('1', p->children[0]);
		return;
	}
	if (p->arity > 1 && STREQ(p->children[1]->symbol, "LP")) {
		if (!searchSymbol(p->children[0]->text))
			symbolErrorMsg('2', p->children[0]);
		return;
	}
	for (i = 0; i < p->arity; i++) {
		if (p->arity > i && STREQ(p->children[i]->symbol, "Exp"))
			procExp(p->children[i]);
	}
}



void buildSymbolTable(TreeNode *p)
{
	if (STREQ(p->symbol, "ExtDef")) {
		// if it is a global definition (can be a function or a variable)
		procExtDef(p);
		return;
	}
	if (STREQ(p->symbol, "Def")) {
		// if it is a local variable definition
		procDef(p);
		return;
	}
	if (STREQ(p->symbol, "Exp")) {
		// if it is a variable use or function call
		procExp(p);
		return;
	}
	if (STREQ(p->symbol, "LC")) {
		// push a new field into the stack
		SymbolStackNode *newstacknode = (SymbolStackNode *) malloc(sizeof(SymbolStackNode));
		newstacknode->next = SymbolStackHead;
		SymbolStackHead = newstacknode;
		SymbolStackHead->SymbolHead = NULL;
	}
	if (STREQ(p->symbol, "RC")) {
		// pop out the field at the top of the stack
		SymbolStackNode *nodetodelete = SymbolStackHead;
		SymbolStackHead = SymbolStackHead->next;
		while (nodetodelete->SymbolHead != NULL) {
			SymbolNode *temp = nodetodelete->SymbolHead;
			if (temp->isfunc && temp->FuncMsg.ArgNum > 0)
				free(temp->FuncMsg.ArgType);
			nodetodelete->SymbolHead = nodetodelete->SymbolHead->StackNext;
			free(temp);
		}
		free(nodetodelete);
	}
	int i;
	for (i = 0; i < p->arity; i++)
		buildSymbolTable(p->children[i]);

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

