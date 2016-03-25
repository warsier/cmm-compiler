%{
	#include "syntax.tab.h"
%}

%option yylineno
/* Tokens */
TAB "	"
SPACE " "
ENTER "\n"

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

TYPE int|float

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
	
{TAB} {;}
{SPACE} {;}
{ENTER} {;}

{STRUCT} {return STRUCT;}
{RETURN} {return RETURN;} 
{IF} {return IF;}
{ELSE} {return ELSE;}
{WHILE} {return WHILE;}

{INT} {return INT;}
{FLOAT} {return FLOAT;}
{ID} {return ID;}
{SEMI} {return SEMI;}
{COMMA} {return COMMA;}
{ASSIGNOP} {return ASSIGNOP;}
{RELOP} {return RELOP;}
{PLUS} {return PLUS;}
{MINUS} {return MINUS;}
{STAR} {return STAR;}
{DIV} {return DIV;}
{AND} {return AND;}
{OR} {return OR;}
{DOT} {return DOT;}
{NOT} {return NOT;}
{LP} {return LP;}
{RP} {return RP;}
{LB} {return LB;}
{RB} {return RB;}
{LC} {return LC;}
{RC} {return RC;}

. { printf("Error type A at line %d: Mysterious characters \'%s\' [lexical error]\n", yylineno, yytext); }


%%

