#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ir.h"
#include "syntax_tree.h"
#include "common.h"

void buildIRTree(TreeNode *p)
{
	int i;
	for (i = 0; i < p->arity; i++)
		buildIRTree(p->children[i]);
}

// the entrance 
void procIR(TreeNode *p)
{
	buildIRTree(p);
}