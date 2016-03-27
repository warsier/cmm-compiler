%{
	#include <stdio.h>
	#include "syntax_tree.h"
	#define YYSTYPE Node* // attribute of all symbols are defined as a pointer towards Node
	
	Node *procNode(Node *p, char *symbol)
	{
		p->symbol = symbol;
		return p;
	}
%}

/* declared tokens */
%nonassoc TAB SPACE ENTER

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB LC RC DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc STRUCT RETURN IF ELSE WHILE
%nonassoc INT FLOAT ID TYPE
%nonassoc SEMI COMMA

%%

/* High-level Definitions */
Program : ExtDefList {
		$$ = procNode(createNode(1, $1), "Program");
		printTree($$, 0);
		deleteTree($$);
	}
	;
ExtDefList : ExtDef ExtDefList {$$ = procNode(createNode(2, $1, $2), "ExtDefList");}
	| /* empty */ {$$ = NULL;}
	;
ExtDef : Specifier ExtDecList SEMI {$$ = procNode(createNode(3, $1, $2, $3), "ExtDef");}
	| Specifier SEMI {$$ = procNode(createNode(2, $1, $2), "ExtDef");}
	| Specifier FunDec CompSt {$$ = procNode(createNode(3, $1, $2, $3), "ExtDef");}
	;   
ExtDecList : VarDec {$$ = procNode(createNode(1, $1), "ExtDecList");}
	| VarDec COMMA ExtDecList {$$ = procNode(createNode(3, $1, $2, $3), "ExtDecList");}
	;

/* Specifiers */
Specifier : TYPE {$$ = procNode(createNode(1, $1), "Specifier");}
	| StructSpecifier {$$ = procNode(createNode(1, $1), "Specifier");}
	;
StructSpecifier : STRUCT OptTag LC DefList RC {$$ = procNode(createNode(5, $1, $2, $3, $4, $5), "StructSpecifier");}
	| STRUCT Tag {$$ = procNode(createNode(2, $1, $2), "StructSpecifier");}
	;
OptTag : ID {$$ = procNode(createNode(1, $1), "OptTag");}
	| /* empty */ {$$ = NULL;}
	;
Tag : ID {$$ = procNode(createNode(1, $1), "Tag");}
	;
	
/* Declarators */
VarDec : ID {$$ = procNode(createNode(1, $1), "VarDec");}
	| VarDec LB INT RB {$$ = procNode(createNode(4, $1, $2, $3, $4), "VarDec");}
	;
FunDec : ID LP VarList RP {$$ = procNode(createNode(4, $1, $2, $3, $4), "FunDec");}
	| ID LP RP {$$ = procNode(createNode(3, $1, $2, $3), "FunDec");}
	;
VarList : ParamDec COMMA VarList {$$ = procNode(createNode(3, $1, $2, $3), "VarList");}
	| ParamDec {$$ = procNode(createNode(1, $1), "VarList");}
	;
ParamDec : Specifier VarDec {$$ = procNode(createNode(2, $1, $2), "ParamDec");}
	;
	
/* Statements */
CompSt : LC DefList StmtList RC {$$ = procNode(createNode(4, $1, $2, $3, $4), "CompSt");}
	;
StmtList : Stmt StmtList {$$ = procNode(createNode(2, $1, $2), "StmtList");}
	| /* empty */ {$$ = NULL;}
	;
Stmt : Exp SEMI {$$ = procNode(createNode(2, $1, $2), "Stmt");}
	| CompSt {$$ = procNode(createNode(1, $1), "Stmt");}
	| RETURN Exp SEMI {$$ = procNode(createNode(3, $1, $2, $3), "Stmt");}
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = procNode(createNode(5, $1, $2, $3, $4, $5), "Stmt");}
	| IF LP Exp RP Stmt ELSE Stmt {$$ = procNode(createNode(7, $1, $2, $3, $4, $5, $6, $7), "Stmt");}
	| WHILE LP Exp RP Stmt {$$ = procNode(createNode(5, $1, $2, $3, $4, $5), "Stmt");}
	;
	
/* Local Definitions */
DefList : Def DefList {$$ = procNode(createNode(2, $1, $2), "DefList");}
	| /* empty */ {$$ = NULL;}
	;
Def : Specifier DecList SEMI {$$ = procNode(createNode(3, $1, $2, $3), "Def");}
	;
DecList : Dec {$$ = procNode(createNode(1, $1), "DecList");}
	| Dec COMMA DecList {$$ = procNode(createNode(3, $1, $2, $3), "DecList");}
	;
Dec : VarDec {$$ = procNode(createNode(1, $1), "Dec");}
	| VarDec ASSIGNOP Exp {$$ = procNode(createNode(3, $1, $2, $3), "Dec");}
	;
	
/* Expressions */
Exp : Exp ASSIGNOP Exp {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| Exp AND Exp {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| Exp OR Exp {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| Exp RELOP Exp {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| Exp PLUS Exp {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| Exp MINUS Exp {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| Exp STAR Exp {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| Exp DIV Exp {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| LP Exp RP {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| MINUS Exp {$$ = procNode(createNode(2, $1, $2), "Exp");}
	| NOT Exp {$$ = procNode(createNode(2, $1, $2), "Exp");}
	| ID LP Args RP {$$ = procNode(createNode(4, $1, $2, $3, $4), "Exp");}
	| ID LP RP {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| Exp LB Exp RB {$$ = procNode(createNode(4, $1, $2, $3, $4), "Exp");}
	| Exp DOT ID {$$ = procNode(createNode(3, $1, $2, $3), "Exp");}
	| ID {$$ = procNode(createNode(1, $1), "Exp");}
	| INT {$$ = procNode(createNode(1, $1), "Exp");}
	| FLOAT {$$ = procNode(createNode(1, $1), "Exp");}
	;
Args : Exp COMMA Args {$$ = procNode(createNode(3, $1, $2, $3), "Args");}
	| Exp {$$ = procNode(createNode(1, $1), "Args");}
	;
	
%%
#include "lex.yy.c"
