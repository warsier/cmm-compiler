#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "syntax_tree.h"
#include "common.h"

#define HASH_MASK (0x3fff) // the size of hash table

// Type of c-- syntax.
typedef struct Type {
	enum { BASIC, ARRAY, STRUCTURE } kind;
	union {
		int basic;
		struct { struct Type *elem; int size; } array;
		struct FieldList *structure;
	} u;
} Type;

// one field in a structure.
typedef struct FieldList {
	char *name;
	struct Type *type;
	struct FieldList *next;
} FieldList;

typedef struct SymbolNode {
	char text[MAX_LEN]; // the text of a symbol
	bool isfunc;
	bool vis;
	int lineno;
	union {
		struct {
			Type RetValType; // type of return value
			Type ArgType; // type of function argument
		} FuncMsg; // if the symbol is a function
		Type VarMsg; // if the symbol is basic type/array/struct
	};
	
	struct SymbolStackNode *toHead;
	struct SymbolNode *Stacknext, *Hashnext;
} SymbolNode;

typedef struct SymbolStackNode {
	struct SymbolNode *SymbolHead;
	struct SymbolStackNode *next;
} SymbolStackNode;

unsigned int hashSymbol(char *name);
void procSymbolTable(TreeNode *p);

#endif