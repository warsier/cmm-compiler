%{
	#include <stdlib.h>
	#include "syntax.tab.h"
	#include "syntax_tree.h"

	// "token" is an enum type, "symbol" is what token looks like
	void procToken(int token, char *symbol)
	{
		Node *p = createNode(0);
		strcpy(p->symbol, symbol);
		strcpy(p->text, yytext);
		p->lineno = yylineno;
		if (strcmp(symbol, "INT") == 0)
			p->intVal = atoi(yytext);
		if (strcmp(symbol, "FLOAT") == 0)
			p->floatVal = atof(yytext);
			
		yylval = p;
	}
%}

%option yylineno
/* Tokens */
BLANK [ \t\n]

TYPE int|float

INT 0|[1-9][0-9]*
FLOAT {INT}\.[0-9]+
ID [_a-zA-Z][_0-9a-zA-Z]*

SEMI ;
COMMA ,
ASSIGNOP =
RELOP <|<=|>|>=|==|!=

PLUS \+
MINUS -
STAR \*
DIV \/

AND &&
OR \|\|
DOT \.
NOT !

LP \(
RP \)
LB \[
RB \]
LC \{
RC \}

STRUCT struct
RETURN return
IF if
ELSE else
WHILE while

%%
	/* output for testing */
	/*
	{TAB} {printf("	");}
	{SPACE} {printf(" ");}
	{ENTER} {printf("\n");}

	{STRUCT} {printf("STRUCT");}
	{RETURN} {printf("RETURN");}
	{IF} {printf("IF");}
	{ELSE} {printf("ELSE");}
	{WHILE} {printf("WHILE");}

	{TYPE} {printf("TYPE");}

	{INT} {printf("INT");}
	{FLOAT} {printf("FLOAT");}
	{ID} {printf("ID");}

	{SEMI} {printf(";");}
	{COMMA} {printf(",");}
	{ASSIGNOP} {printf("=");}
	{RELOP} {printf("RELOP");}

	{PLUS} {printf("+");}
	{MINUS} {printf("-");}
	{STAR} {printf("*");}
	{DIV} {printf("/");}

	{AND} {printf("&&");}
	{OR} {printf("||");}
	{DOT} {printf(".");}
	{NOT} {printf("!");}

	{LP} {printf("(");}
	{RP} {printf(")");}
	{LB} {printf("[");}
	{RB} {printf("]");}
	{LC} {printf("{");}
	{RC} {printf("}");}

	. { printf("\nError type A at line %d: Mysterious characters \'%s\' [lexical error]\n", yylineno, yytext); }
	*/


	/* output for syntax analysis */
	
{BLANK} {;}

{STRUCT} {proc(STRUCT, "STRUCT"); return STRUCT;}
{RETURN} {proc(RETURN, "RETURN"); return RETURN;} 
{IF} {proc(IF, "IF"); return IF;}
{ELSE} {proc(ELSE, "ELSE"); return ELSE;}
{WHILE} {proc(WHILE, "WHILE"); return WHILE;}

{TYPE} {proc(TYPE, "TYPE"); return TYPE;}

{INT} {proc(INT, "INT"); return INT;}
{FLOAT} {proc(FLOAT, "FLOAT"); return FLOAT;}
{ID} {proc(ID, "ID"); return ID;}

{SEMI} {proc(SEMI, "SEMI"); return SEMI;}
{COMMA} {proc(COMMA, "COMMA"); return COMMA;}
{ASSIGNOP} {proc(ASSIGNOP, "ASSIGNOP"); return ASSIGNOP;}
{RELOP} {proc(RELOP, "RELOP"); return RELOP;}

{PLUS} {proc(PLUS, "PLUS"); return PLUS;}
{MINUS} {proc(MINUS, "MINUS"); return MINUS;}
{STAR} {proc(STAR, "STAR"); return STAR;}
{DIV} {proc(DIV, "DIV"); return DIV;}

{AND} {proc(AND, "AND"); return AND;}
{OR} {proc(OR, "OR"); return OR;}
{DOT} {proc(DOT, "DOT"); return DOT;}
{NOT} {proc(NOT, "NOT"); return NOT;}


{LP} {proc(LP, "LP"); return LP;}
{RP} {proc(RP, "RP"); return RP;}
{LB} {proc(LB, "LB"); return LB;}
{RB} {proc(RB, "RB"); return RB;}
{LC} {proc(LC, "LC"); return LC;}
{RC} {proc(RC, "RC"); return RC;}

. { printf("Error type A at line %d: Mysterious characters \'%s\' [lexical error]\n", yylineno, yytext); }

%%

