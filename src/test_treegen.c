#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "syntax_tree.h"

extern Node *createNode(int arity, ...);
extern int deleteNode(Node *p);
extern void printTree(Node *p, int depth);
extern void deleteTree(Node *p);

int main()
{
	Node *p = createNode(0);
	strcpy(p->symbol, "TYPE");
	strcpy(p->name, "int");
	Node *q = createNode(0);
	strcpy(q->symbol, "INT");
	q->intVal = 15;
	Node *r = createNode(2, p, q);
	printTree(r, 0);
	deleteTree(r);
	return 0;
}