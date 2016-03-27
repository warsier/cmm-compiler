#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include "syntax_tree.h"

Node *createNode(int arity, ...)
{
	assert(arity <= MAX_CHILDREN);
	Node *p = (Node *) malloc(sizeof(Node));
	p->symbol = (char *) malloc(MAX_LEN);
	p->text = (char *) malloc(MAX_LEN);
	p->arity = arity;
	
	va_list arg_ptr;
	va_start(arg_ptr, arity);
	int i;
	for (i = 0; i < p->arity; i++) {
		Node *temp = va_arg(arg_ptr, Node *);
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

int deleteNode(Node *p)
{
	printf("%s\n", p->symbol);
	free(p->symbol);
	free(p->text);
	int i;
	for (i = 0; i < p->arity; i++) {
		free(p->children[i]);
	}
	free(p);
	p = NULL;
	return 0;
}

void printTree(Node *p, int depth)
{
	printf("%*s%s", depth * INDENT, "", p->symbol);
	if (p->arity == 0) {
		if (strcmp(p->symbol, "TYPE") == 0)
			printf(": %s\n", p->text);
		else if (strcmp(p->symbol, "ID") == 0)
			printf(": %s\n", p->text);
		else if (strcmp(p->symbol, "RELOP") == 0)
			printf(": %s\n", p->text);
		else if (strcmp(p->symbol, "INT") == 0)
			printf(": %d\n", p->intVal);
		else if (strcmp(p->symbol, "FLOAT") == 0)
			printf(": %f\n", p->floatVal);
		else
			printf("\n");
	}
	else {
		printf(" (%d)\n", p->lineno);
		int i;
		for (i = 0; i < p->arity; i++) {
			printTree(p->children[i], depth + 1);
		}
	}
}

void deleteTree(Node *p)
{
	int i;
	for (i = 0; i < p->arity; i++)
		deleteTree(p->children[i]);
	deleteNode(p);
}
