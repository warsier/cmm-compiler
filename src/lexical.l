%{
	#include <stdlib.h>
	#include "syntax.tab.h"
	#include "syntax_tree.h"
	
	bool errorStat = false;

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

	. { printf("\nError type A at line %d: Mysterious character \'%s\'. [lexical error]\n", yylineno, yytext); }
	*/


	/* output for syntax analysis */
	
{BLANK} {;}

{STRUCT} {procToken(STRUCT, "STRUCT"); return STRUCT;}
{RETURN} {procToken(RETURN, "RETURN"); return RETURN;} 
{IF} {procToken(IF, "IF"); return IF;}
{ELSE} {procToken(ELSE, "ELSE"); return ELSE;}
{WHILE} {procToken(WHILE, "WHILE"); return WHILE;}

{TYPE} {procToken(TYPE, "TYPE"); return TYPE;}

{INT} {procToken(INT, "INT"); return INT;}
{FLOAT} {procToken(FLOAT, "FLOAT"); return FLOAT;}
{ID} {procToken(ID, "ID"); return ID;}

{SEMI} {procToken(SEMI, "SEMI"); return SEMI;}
{COMMA} {procToken(COMMA, "COMMA"); return COMMA;}
{ASSIGNOP} {procToken(ASSIGNOP, "ASSIGNOP"); return ASSIGNOP;}
{RELOP} {procToken(RELOP, "RELOP"); return RELOP;}

{PLUS} {procToken(PLUS, "PLUS"); return PLUS;}
{MINUS} {procToken(MINUS, "MINUS"); return MINUS;}
{STAR} {procToken(STAR, "STAR"); return STAR;}
{DIV} {procToken(DIV, "DIV"); return DIV;}

{AND} {procToken(AND, "AND"); return AND;}
{OR} {procToken(OR, "OR"); return OR;}
{DOT} {procToken(DOT, "DOT"); return DOT;}
{NOT} {procToken(NOT, "NOT"); return NOT;}


{LP} {procToken(LP, "LP"); return LP;}
{RP} {procToken(RP, "RP"); return RP;}
{LB} {procToken(LB, "LB"); return LB;}
{RB} {procToken(RB, "RB"); return RB;}
{LC} {procToken(LC, "LC"); return LC;}
{RC} {procToken(RC, "RC"); return RC;}

. { 
	printf("Error type A at line %d: Mysterious characters \'%s\'. [lexical error]\n", yylineno, yytext); 
	errorStat = true;
}

%%

