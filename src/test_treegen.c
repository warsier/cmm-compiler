#include <stdio.h>
#include <assert.h>
#include "../include/syntax_tree.h"

int main()
{
	Node *p = createNode(0);
	Node *q = createNode(0);
	Node *r = createNode(2, p, q);
	printTree(r, 0);
	deleteTree(r);
	assert(p == NULL);
	assert(q == NULL);
	assert(r == NULL);
	return 0;
}