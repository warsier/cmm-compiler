%{
	#include <stdio.h>
%}

/* declared tokens */
%token TAB SPACE ENTER
%token STRUCT RETURN IF ELSE WHILE
%token INT FLOAT ID
%token SEMI COMMA ASSIGNOP RELOP
%token PLUS MINUS STAR DIV
%token AND OR DOT NOT
%token LP RP LB RB LC RC

%%

/* High-level Definitions */
Program : ExtDefList
	;
ExtDefList : ExtDef ExtDefList
	| /* empty */
	;
ExtDef : Specifier ExtDecList SEMI
	| Specifier SEMI
	| Specifier FunDec CompSt
	;   
ExtDecList : VarDec
	| VarDec COMMA ExtDecList
	;

/* Specifiers */
Specifier : TYPE
	| StructSpecifier
	;
StructSpecifier : STRUCT OptTag LC DefList RC
	| STRUCT Tag
	;
OptTag : ID
	| /* empty */
	;
Tag : ID
	;
	
/* Declarators */
VarDec : ID
	| VarDec LB INT RB
	;
FunDec : ID LP VarList RP
	| ID LP RP
	;
VarList : ParamDec COMMA VarList
	| ParamDec
	;
ParamDec : Specifier VarDec
	;
	
/* Statements */
CompSt : LC DefList StmtList RC
	;
StmtList : Stmt StmtList
	| /* empty */
	;
Stmt : Exp SEMI
	| CompSt
	| RETURN Exp SEMI
	| IF LP Exp RP Stmt
	| IF LP Exp RP Stmt ELSE Stmt
	| WHILE LP Exp RP Stmt
	;
	
/* Local Definitions */
DefList : Def DefList
	| /* empty */
	;
Def : Specifier DecList SEMI
	;
DecList : Dec
	| Dec COMMA DecList
	;
Dec : VarDec
	| VarDec ASSIGNOP Exp
	;
	
/* Expressions */
Exp : Exp ASSIGNOP Exp
	| Exp AND Exp
	| Exp OR Exp
	| Exp RELOP Exp
	| Exp PLUS Exp
	| Exp MINUS Exp
	| Exp STAR Exp
	| Exp DIV Exp
	| LP Exp RP
	| MINUS Exp
	| NOT Exp
	| ID LP Args RP
	| ID LP RP
	| Exp LB Exp RB
	| Exp DOT ID
	| ID
	| INT
	| FLOAT
	;
Args : Exp COMMA Args
	| Exp
	;

TYPE : INT
	| FLOAT
	; 
	
%%
#include "lex.yy.c"
