// line 523 in lex.yy.c: lexical.l part 1
#include <stdlib.h>
#include <assert.h>
#include "syntax.tab.h"
#include "syntax_tree.h"

// "token" is an enum type, "symbol" is what token looks like
void procToken(int token, char *symbol)
{
	Node *p = createNode(0);
	strcpy(p->symbol, symbol);
	strcpy(p->text, yytext);
	assert(strlen(yytext) <= MAX_LEN);
	p->lineno = yylineno;
	if (strcmp(symbol, "INT") == 0)
		p->intVal = atoi(yytext);
	if (strcmp(symbol, "FLOAT") == 0)
		p->floatVal = atof(yytext);
		
	yylval = p;
}

// line 65 in syntax.tab.c: syntax.y part 1
#include <stdio.h>
#include "syntax.tab.h"
#include "syntax_tree.h"
#define YYSTYPE Node* // attribute of all symbols are defined as a pointer towards Node

Node *procNode(Node *p, char *symbol)
{
	p->symbol = symbol;
	return p;
}

// line 1918 in syntax.tab.c: syntax.y part 3
#include "lex.yy.c"