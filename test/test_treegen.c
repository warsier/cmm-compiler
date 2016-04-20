#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "syntax_tree.h"

extern TreeNode *createTreeNode(int arity, ...);
extern int deleteTreeNode(TreeNode *p);
extern void printTree(TreeNode *p, int depth);
extern void deleteTree(TreeNode *p);

int main()
{
	TreeNode *p = createTreeNode(0);
	strcpy(p->symbol, "TYPE");
	strcpy(p->text, "int");
	TreeNode *q = createTreeNode(0);
	strcpy(q->symbol, "INT");
	q->intVal = 15;
	TreeNode *r = createTreeNode(2, p, q);
	printTree(r, 0);
	deleteTree(r);
	return 0;
}