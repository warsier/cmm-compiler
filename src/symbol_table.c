#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symbol_table.h"
#include "syntax_tree.h"
#include "common.h"

SymbolNode *HashTable[HASH_MASK];
SymbolStackNode *SymbolStackHead = NULL;
SymbolNode *StructTableHead = NULL;

void symbolErrorMsg(char ErrorType, TreeNode *p)
{
	switch(ErrorType) {
	case '1': printf("Error type 1 at line %d: Undefined variable \"%s\".\n", p->lineno, p->text); break;
	case '2': printf("Error type 2 at line %d: Undefined function \"%s\".\n", p->lineno, p->text); break;
	case '3': printf("Error type 3 at line %d: Redefined variable \"%s\".\n", p->lineno, p->text); break;
	case '4': printf("Error type 4 at line %d: Redefined function \"%s\".\n", p->lineno, p->text); break;
	case '5': printf("Error type 5 at line %d: Type mismatched for assignment.\n", p->lineno); break;
	case '6': printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable.\n", p->lineno); break;
	case '7': printf("Error type 7 at line %d: Type mismatched for operands.\n", p->lineno); break;
	case '8': printf("Error type 8 at line %d: Type mismatched for return.\n", p->lineno); break;
	case '9': printf("Error type 9 at line %d: ", p->lineno); break;
	case 'a': printf("Error type 10 at line %d: \"%s\" is not an array.\n", p->lineno, p->text); break;
	case 'b': printf("Error type 11 at line %d: \"%s\" is not a function.\n", p->lineno, p->text); break;
	case 'c': printf("Error type 12 at line %d: \"%s\" is not an integer.\n", p->lineno, p->text); break;
	case 'd': printf("Error type 13 at line %d: Illegal use of \".\".\n", p->lineno); break;
	}
}

void printType(Type t, char *str)
{
	if (t.kind == BASIC) {
		if (t.basic == B_INT) strcpy(str, "int\0");
		else strcpy(str, "float\0");
	}
	else {
		assert(0);
	}
}

void deleteType(Type *t)
{
	if (t->kind == BASIC) {
		free(t);
	}
	if (t->kind == ARRAY) {
		Type *temp = t;
		while (t->kind != BASIC) {
			t = t->array.elem;
			free(temp);
			temp = t;
		}
		free(t);
	}
	if (t->kind == STRUCTURE) {
		while (t->structure != NULL) {
			FieldList *temp = t->structure;
			t->structure = t->structure->next;
			free(temp);
		}
		free(t);
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

SymbolNode *searchSymbol(const char *name)
{
	SymbolNode *temp = HashTable[hashSymbol(name)]; // generate the hash slot
	for (; temp != NULL; temp = temp->HashNext) {
		if (STREQ(temp->text, name))
			return temp;
	}
	return NULL;
}

SymbolNode *searchStructTable(const char *name)
{
	SymbolNode *temp = StructTableHead;
	for (; temp != NULL; temp = temp->StackNext) {
		if (STREQ(temp->text, name))
			return temp;
	}
	return NULL;
}

FieldList *searchStructField(const char *name)
{
	FieldList *temp = StructTableHead->VarMsg->structure;
	for (; temp != NULL; temp = temp->next) {
		if (STREQ(temp->name, name))
			return temp;
	}
	return NULL;
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

SymbolNode *pushinStruct(const char *name)
{
	SymbolNode *newnode = (SymbolNode *) malloc(sizeof(SymbolNode));
	if (StructTableHead == NULL) {
		StructTableHead = newnode;
		newnode->StackNext = NULL;
	}
	else {
		newnode->StackNext = StructTableHead;
		StructTableHead = newnode;
	}
	
	return newnode;
}

FieldList *pushinStructField(const char *name)
{
	FieldList *newnode = (FieldList *) malloc(sizeof(FieldList));
	if (StructTableHead->VarMsg->structure == NULL) {
		StructTableHead->VarMsg->structure = newnode;
		newnode->next = NULL;
	}
	else {
		newnode->next = StructTableHead->VarMsg->structure;
		StructTableHead->VarMsg->structure = newnode;
	}
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
			if (!temp->isfunc)
				deleteType(temp->VarMsg);
			p->SymbolHead = p->SymbolHead->StackNext;
			free(temp);
		}
		free(p);
	}
	int i;
	for (i = 0; i < HASH_MASK; i++)
		HashTable[i] = NULL;
	 
}

void clearStructTable()
{
	while (StructTableHead != NULL) {
		SymbolNode *p = StructTableHead;
		StructTableHead = StructTableHead->StackNext;
		if (p->isfunc && p->FuncMsg.ArgNum > 0)
			free(p->FuncMsg.ArgType);
		if (!p->isfunc)
			deleteType(p->VarMsg);
		free(p);
	}
}

int TypeEQ(Type lval, Type rval)
{
	if (lval.kind == NOTDEF || rval.kind == NOTDEF) return -1;
	if (lval.kind != rval.kind) return 0;
	if (lval.kind == BASIC) {
		if (lval.basic != rval.basic) return 0;
	}
	else if (lval.kind == ARRAY) {
		if (lval.array.size != rval.array.size) return 0;
		if (TypeEQ(*lval.array.elem, *rval.array.elem) == 0) return 0;
	}
	else {
		FieldList *llist = lval.structure, *rlist = rval.structure;
		while (llist != NULL && rlist != NULL) {
			if (TypeEQ(*llist->type, *rlist->type) == 0) return 0;
			llist = llist->next, rlist = rlist->next;
		}
		if (!(llist == NULL && rlist == NULL)) return 0;
	}
	return 1;
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
	else if (STREQ(p->children[1]->symbol, "SEMI")) {
		procSpecifier(p->children[0]);
	}
}

// FunDec -> ID LP VarList RP
//         | ID LP RP
// VarList -> ParamDec COMMA VarList
//          | ParamDec
// ParamDec -> Specifier VarDec
void procFunDec(Type nodetype, TreeNode *p)
{
	SymbolNode *symboltemp = searchSymbol(p->children[0]->text);
	if (symboltemp != NULL)
	{
		symbolErrorMsg('4', p->children[0]);
		pushSymbolStack();
		return;
	}
	
	SymbolNode *newnode = pushinSymbol(p->children[0]->text);
	strcpy(newnode->text, p->children[0]->text);
	newnode->isfunc = true, newnode->isdef = true;
	newnode->lineno = p->children[0]->lineno;
	newnode->FuncMsg.RetValType = nodetype;
	
	pushSymbolStack();
	SymbolStackHead->funcptr = newnode;
	
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
	Type nodetype = procSpecifier(p->children[0]);
	TreeNode *temp = p->children[1];
	while (temp->arity > 1) {
		procVarDec(nodetype, temp->children[0]->children[0]);
		temp = temp->children[2];
	}
	procVarDec(nodetype, temp->children[0]->children[0]);
}

void procStructDef(TreeNode *p)
{
	Type nodetype = procSpecifier(p->children[0]);
	TreeNode *temp = p->children[1];
	while (temp->arity > 1) {
		procStructVarDec(nodetype, temp->children[0]->children[0]);
		temp = temp->children[2];
	}
	procStructVarDec(nodetype, temp->children[0]->children[0]);
}

// Specifier -> TYPE 
//            | StructSpecifier
// StructSpecifier -> STRUCT OptTag LC DefList RC
//                  | STRUCT Tag
// OptTag -> ID
//         | /* empty */
// Tag -> ID
Type procSpecifier(TreeNode *p)
{
	Type nodetype;
	if (STREQ(p->children[0]->symbol, "TYPE")) {
		nodetype.kind = BASIC;
		if (STREQ(p->children[0]->text, "int")) nodetype.basic = B_INT;
		else nodetype.basic = B_FLOAT;
	}
	else if (STREQ(p->children[0]->symbol, "StructSpecifier")) {
		TreeNode *structtemp = p->children[0];
		if (structtemp->arity > 3) {
			SymbolNode *newnode = pushinStruct(structtemp->children[1]->children[0]->text);
			strcpy(newnode->text, structtemp->children[1]->children[0]->text);
			newnode->VarMsg = (Type *) malloc(sizeof(Type));
			newnode->VarMsg->kind = STRUCTURE;
			TreeNode *deftemp = structtemp->children[3];
			while (deftemp->arity > 1) {
				procStructDef(deftemp->children[0]);
				deftemp = deftemp->children[1];
			}
			nodetype = *newnode->VarMsg;
		}
		else {
			
		}
	}
	else {
		assert(0);
	}
	return nodetype;
}

// VarDec -> ID
//         | VarDec LB INT RB
void procVarDec(Type nodetype, TreeNode *p)
{
	if (p->arity == 1) {
		if (searchSymbol(p->children[0]->text) != NULL) {
			symbolErrorMsg('3', p->children[0]);
			return;
		}
		SymbolNode *newnode = pushinSymbol(p->children[0]->text);
		strcpy(newnode->text, p->children[0]->text);
		newnode->isfunc = false, newnode->isdef = true;
		newnode->lineno = p->children[0]->lineno;
		newnode->VarMsg = (Type *) malloc(sizeof(Type));
		newnode->VarMsg->kind = BASIC;
		newnode->VarMsg->basic = nodetype.basic;
	}
	else {
		TreeNode *temp = p;
		Type *arrayhead = (Type *) malloc(sizeof(Type));
		arrayhead->kind = BASIC;
		arrayhead->basic = nodetype.basic;
		while (temp->arity > 1) {
			Type *arraytemp = (Type *) malloc(sizeof(Type));
			arraytemp->kind = ARRAY;
			arraytemp->array.size = temp->children[2]->intVal;
			arraytemp->array.elem = arrayhead;
			arrayhead = arraytemp;
			temp = temp->children[0];
		}
		if (searchSymbol(temp->children[0]->text) != NULL) {
			symbolErrorMsg('3', temp->children[0]);
			return;
		}
		SymbolNode *newnode = pushinSymbol(temp->children[0]->text);
		strcpy(newnode->text, temp->children[0]->text);
		newnode->isfunc = false, newnode->isdef = true;
		newnode->lineno = p->children[0]->lineno;
		newnode->VarMsg = arrayhead;
	}
}

void procStructVarDec(Type nodetype, TreeNode *p)
{
	if (p->arity == 1) {
		if (searchStructField(p->children[0]->text) != NULL) {
			symbolErrorMsg('3', p->children[0]);
			return;
		}
		FieldList *newnode = pushinStructField(p->children[0]->text);
	}
	else {
	}
}

// Stmt -> Exp SEMI
//       | CompSt
//       | RETURN Exp SEMI
//       | IF LP Exp RP Stmt
//       | IF LP Exp RP Stmt ELSE Stmt
//       | WHILE LP Exp RP Stmt
void procStmt(TreeNode *p)
{
	if (STREQ(p->children[0]->symbol, "RETURN")) {
		SymbolStackNode *funcfield = SymbolStackHead;
		while (funcfield != NULL && funcfield->funcptr == NULL) 
			funcfield = funcfield->next;
		if (funcfield != NULL && TypeEQ(funcfield->funcptr->FuncMsg.RetValType, procExp(p->children[1])) == 0)
			symbolErrorMsg('8', p);
	}
	int i;
	for (i = 0; i < p->arity; i++)
		buildSymbolTable(p->children[i]);
}

Type procExp(TreeNode *p)
{
	Type retval;
	retval.kind = OTHER;
	// single element
	if (p->arity == 1) {
		if (STREQ(p->children[0]->symbol, "ID")) {
			SymbolNode *symboltemp = searchSymbol(p->children[0]->text);
			SymbolNode *structtemp = searchStructTable(p->children[0]->text);
			if(symboltemp == NULL && structtemp == NULL) {
				symbolErrorMsg('1', p->children[0]);
				retval.kind = NOTDEF;
				return retval;
			}
			else if (symboltemp != NULL) {
				retval = *symboltemp->VarMsg;
			}
			else if (structtemp != NULL) {
				retval = *structtemp->VarMsg;
			}
			return retval;
		}
		else if (STREQ(p->children[0]->symbol, "INT")) {
			retval.kind = BASIC;
			retval.basic = B_INT;
		}
		else {
			retval.kind = BASIC;
			retval.basic = B_FLOAT;
		}
		return retval;
	}
	
	// array
	if (p->arity > 2 && STREQ(p->children[1]->symbol, "LB")) {
		TreeNode *temp = p;
		while (temp->arity > 1)
			temp = temp->children[0];
		SymbolNode *symboltemp = searchSymbol(temp->children[0]->text);
		if(symboltemp == NULL) {
			symbolErrorMsg('1', p->children[0]);
			retval.kind = NOTDEF;
			return retval;
		}
		else if (symboltemp->VarMsg->kind != ARRAY) {
			symbolErrorMsg('a', temp->children[0]);
			retval.kind = NOTDEF;
			return retval;
		}
		temp = p;
		while (temp->arity > 1) {
			Type inttemp;
			inttemp.kind = BASIC, inttemp.basic = B_INT;
			if (TypeEQ(procExp(temp->children[2]), inttemp) == 0) {
				symbolErrorMsg('c', temp->children[2]->children[0]);
				retval.kind = NOTDEF;
				return retval;
			}
			temp = temp->children[0];
		}
	}
	
	// struct
	if (p->arity > 1 && STREQ(p->children[1]->symbol, "DOT")) {
		if (procExp(p->children[0]).kind == BASIC || procExp(p->children[0]).kind == NOTDEF) {
			symbolErrorMsg('d', p->children[0]);
			retval.kind = NOTDEF;
			return retval;
		}
		return retval;
	}
	
		
	// function
	// Exp -> ID LP Args RP
	//      | ID LP RP
	// Args -> Exp COMMA Args
	//       | Exp
	if (p->arity > 1 && STREQ(p->children[1]->symbol, "LP")) {
		SymbolNode *symboltemp = searchSymbol(p->children[0]->text);
		if (symboltemp == NULL) 
			symbolErrorMsg('2', p->children[0]);
		else if (symboltemp->isfunc == false)
			symbolErrorMsg('b', p->children[0]);
		else {
			int cnt = 1;
			TreeNode *argstemp = p->children[2];
			while (argstemp->arity > 1) {
				cnt++;
				argstemp = argstemp->children[2];
			}
			Type *call = (Type *) malloc(sizeof(Type) * cnt);
			cnt = 0;
			argstemp = p->children[2];
			while (argstemp->arity > 1) {
				call[cnt] = procExp(argstemp->children[0]);
				cnt++;
				argstemp = argstemp->children[2];
			}
			call[cnt] = procExp(argstemp->children[0]);
			cnt++;
			bool flag = true;
			if (cnt != symboltemp->FuncMsg.ArgNum)
				flag = false;
			else {
				int i;
				for (i = 0; i < cnt; i++)
					if (TypeEQ(call[cnt], symboltemp->FuncMsg.ArgType[cnt]) == 0) {
						flag = false;
						break;
					}
			}
			if (!flag) {
				symbolErrorMsg('9', p);
				printf("Function \"%s(", symboltemp->text);
				int i;
				char str[40];
				for (i = 0; i < symboltemp->FuncMsg.ArgNum - 1; i++) {
					printType(symboltemp->FuncMsg.ArgType[i], str);
					printf("%s, ", str);
				}
				printType(symboltemp->FuncMsg.ArgType[i], str);
				printf("%s", str);
				printf(")\" is not applicable for arguments \"(");
				for (i = 0; i < cnt - 1; i++) {
					printType(call[i], str);
					printf("%s, ", str);
				}
				printType(call[i], str);
				printf("%s)\".\n", str);
			}
			free(call);
		}
		return retval;
	}
	
	// three elements
	if (p->arity == 3 && STREQ(p->children[0]->symbol, "LP")) { // brackets
		return procExp(p->children[1]);
	}
	if (p->arity == 3 && STREQ(p->children[1]->symbol, "ASSIGNOP")) {
		bool flag = false;
		if (STREQ(p->children[0]->children[0]->symbol, "ID")) flag = true;
		if (p->children[0]->arity > 1 && STREQ(p->children[0]->children[1]->symbol, "LB")) flag = true;
		if (p->children[0]->arity > 1 && STREQ(p->children[0]->children[1]->symbol, "DOT")) flag = true;
		if (!flag) {
			symbolErrorMsg('6', p);
			return retval;
		}
		Type lval = procExp(p->children[0]), rval = procExp(p->children[2]);
		if (TypeEQ(lval, rval) == 0) {
			symbolErrorMsg('5', p);
			return retval;
		}
		return lval;
	}
	if (p->arity == 3 && !STREQ(p->children[1]->symbol, "Exp") && !STREQ(p->children[1]->symbol, "DOT")) {
		Type lval = procExp(p->children[0]), rval = procExp(p->children[2]);	
		if (TypeEQ(lval, rval) == 0) {
			symbolErrorMsg('7', p);
			return retval;
		}
		return lval;
	}
	int i;
	for (i = 0; i < p->arity; i++) {
		if (p->arity > i && STREQ(p->children[i]->symbol, "Exp"))
			procExp(p->children[i]);
	}
	return retval;
}

void buildSymbolTable(TreeNode *p)
{
	if (STREQ(p->symbol, "ExtDef")) {
		// if it is a global definition (can be a function or a variable)
		procExtDef(p);
		return;
	}
	if (STREQ(p->symbol, "StructSpecifier")) {
		
	}
	if (STREQ(p->symbol, "Def")) {
		// if it is a local variable definition
		procDef(p);
		return;
	}
	if (STREQ(p->symbol, "Stmt")) {
		// if it is a local variable definition
		procStmt(p);
		return;
	}
	if (STREQ(p->symbol, "Exp")) {
		// if it is a variable use or function call
		procExp(p);
		return;
	}
	if (STREQ(p->symbol, "LC")) {
		pushSymbolStack();
	}
	if (STREQ(p->symbol, "RC")) {
		popSymbolStack();
	}
	int i;
	for (i = 0; i < p->arity; i++)
		buildSymbolTable(p->children[i]);

}

// the entrance
void procSymbolTable(TreeNode *p)
{
	clearSymbolStack();
	SymbolStackHead = (SymbolStackNode *) malloc(sizeof(SymbolStackNode)); // global symbol table
	SymbolStackHead->funcptr = NULL;
	SymbolStackHead->next = NULL;
	SymbolStackHead->SymbolHead = NULL;
	buildSymbolTable(p);
	clearSymbolStack();
	clearStructTable();
}

void pushSymbolStack()
{
	// push a new field into the stack
	SymbolStackNode *newstacknode = (SymbolStackNode *) malloc(sizeof(SymbolStackNode));
	newstacknode->next = SymbolStackHead;
	SymbolStackHead = newstacknode;
	SymbolStackHead->funcptr = NULL;
	SymbolStackHead->SymbolHead = NULL;
}

void popSymbolStack()
{
	// pop out the field at the top of the stack
	SymbolStackNode *nodetodelete = SymbolStackHead;
	SymbolStackHead = SymbolStackHead->next;
	while (nodetodelete->SymbolHead != NULL) {
		SymbolNode *temp = nodetodelete->SymbolHead;
		if (temp->isfunc && temp->FuncMsg.ArgNum > 0)
			free(temp->FuncMsg.ArgType);
		if (!temp->isfunc)
			deleteType(temp->VarMsg);
		nodetodelete->SymbolHead = nodetodelete->SymbolHead->StackNext;
		free(temp);
	}
	free(nodetodelete);
}

