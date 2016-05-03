#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "syntax_tree.h"
#include "common.h"

#define HASH_MASK (0x3fff) // the size of hash table

// Type of c-- syntax.
typedef struct Type {
	enum { BASIC, ARRAY, STRUCTURE, OTHER, NOTDEF } kind;
	union {
		enum { B_INT = 1, B_FLOAT = 2 } basic; 
		struct { struct Type *elem; int size; } array;
		struct FieldList *structure;
	};
} Type;

// one field in a structure.
typedef struct FieldList {
	char name[MAX_LEN];
	struct Type *type;
	struct FieldList *next;
} FieldList;

typedef struct SymbolNode {
	char text[MAX_LEN]; // the text of a symbol
	bool isfunc; // the symbol is a function or a variable
	bool isdef; // this symbol is a definition or a declaration (we don't need to consider extern variables, so isdef is always true for variables)
	int lineno;
	union {
		struct {
			Type RetValType; // type of return value
			int ArgNum;
			Type *ArgType; // type of function argument
		} FuncMsg; // if the symbol is a function
		Type *VarMsg; // if the symbol is basic type/array/struct
	};
	
	struct SymbolNode *StackNext, *HashNext;
} SymbolNode;

typedef struct SymbolStackNode {
	struct SymbolNode *SymbolHead;
	struct SymbolNode *funcptr;
	struct SymbolStackNode *next;
} SymbolStackNode;

void symbolErrorMsg(char ErrorType, TreeNode *p);
void printType(Type t, char *str);
void deleteType(Type *t);
unsigned int hashSymbol(const char *name);
SymbolNode *searchSymbol(const char *name);
SymbolNode *searchStructTable(const char *name);
FieldList *searchStructField(FieldList *structure, const char *name);
SymbolNode *pushinSymbol(const char *name);
SymbolNode *pushinStruct(const char *name);
FieldList *pushinStructField(const char *name);
void clearSymbolStack();
void clearStructTable();
int TypeEQ(Type lval, Type rval);
void procExtDef(TreeNode *p);
void procFunDec(Type nodetype, TreeNode *p);
void procDef(TreeNode *p);
void procStructDef(TreeNode *p);
Type procSpecifier(TreeNode *p);
void procVarDec(Type nodetype, TreeNode *p);
void procStructVarDec(Type nodetype, TreeNode *p);
Type procExp(TreeNode *p);
void buildSymbolTable(TreeNode *p);
void procSymbolTable(TreeNode *p);
void pushSymbolStack();
void popSymbolStack();

#endif