#ifndef __SYNTAX_TREE_H__
#define __SYNTAX_TREE_H__

typedef unsigned bool;
#define true 1
#define false 0

#define MAX_CHILDREN 10 // the number of children in c-- syntax will not exceed MAX_CHILDREN
#define MAX_LEN 30 // the length of symbol and name shall not exceed MAX_LEN

#define INDENT 2 // control the output indent of the syntax tree
#define ALWAYS_OUTPUT_TREE 0 // used to enable syntax tree output in testing, default value is 0

typedef struct Node
{
	char symbol[MAX_LEN], text[MAX_LEN];
	// "symbol" stores the kind of the symbols, while "text" stores the actual text the symbol have in the program, like an INT may have value "15", and a TYPE may be "float". Note that the "text" value of a nonterminal is the same to "symbol".
	int lineno;
	union {
		int intVal;
		float floatVal;
	};
	int arity; // if arity > 0, then this Node is nonterminal, otherwise it is terminal
	struct Node *children[MAX_CHILDREN];
} Node;

extern Node *createNode(int arity, ...);
extern void printTree(Node *p, int depth);
extern void deleteNode(Node *p);

#endif