#ifndef __SYNTAX_TREE_H__
#define __SYNTAX_TREE_H__

typedef unsigned bool;
#define true 1
#define false 0
#define MAX_CHILDREN 10

typedef struct Node
{
	bool isTerminal;
	char *symbol, *name;
	// "symbol" stores the kind of the symbols, while "name" stores the specific name of the symbol in the program.
	int lineno;
	union {
		int intVal;
		float floatVal;
	};
	int arity;
	Node *children[MAX_CHILDREN];
	
} Node;

#endif