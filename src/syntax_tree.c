#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include "syntax_tree.h"
#include "common.h"

TreeNode *createTreeNode(int arity, ...)
{
	assert(arity <= MAX_CHILDREN);
	TreeNode *p = (TreeNode *) malloc(sizeof(TreeNode));
	p->arity = arity;
	
	va_list arg_ptr;
	va_start(arg_ptr, arity);
	int i;
	for (i = 0; i < p->arity; i++) {
		TreeNode *temp = va_arg(arg_ptr, TreeNode *);
		if (temp != NULL) {
			p->children[i] = temp;
		}
		else {
			i--; p->arity--;
			/*
			used to deal with recursive definitions like:
			StmtList : Stmt StmtList 
				|
				;
			*/
		}
	}
		
	if (p->arity > 0)
		p->lineno = p->children[0]->lineno;
	
	return p;
}

void printTree(TreeNode *p, int depth)
{
	printf("%*s%s", depth * INDENT, "", p->symbol);
	if (p->arity == 0) {
		if (STREQ(p->symbol, "TYPE"))
			printf(": %s\n", p->text);
		else if (STREQ(p->symbol, "ID"))
			printf(": %s\n", p->text);
		else if (STREQ(p->symbol, "RELOP"))
			printf(": %s\n", p->text);
		else if (STREQ(p->symbol, "INT"))
			printf(": %d\n", p->intVal);
		else if (STREQ(p->symbol, "FLOAT"))
			printf(": %f\n", p->floatVal);
		else
			printf("\n");
	}
	else {
		printf(" (%d)\n", p->lineno);
		int i;
		for (i = 0; i < p->arity; i++) 
			printTree(p->children[i], depth + 1);
	}
}

void deleteTreeNode(TreeNode *p)
{
	int i;
	for (i = 0; i < p->arity; i++)
		deleteTreeNode(p->children[i]);
	free(p);
	p = NULL;
}
