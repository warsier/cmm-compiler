#ifndef __IR_H__
#define __IR_H__

#include "common.h"
#define OUTPUT_TO_SCREEN 1

typedef struct Operand {
	enum { VARIABLE, TEMP, CONSTANT, ADDRESS } kind;
	union {
		int var_no;
		int value;
	};
} Operand;

typedef struct InterCode {
	enum { ASSIGN, ADD, SUB, MUL, DIV_ } kind;
	union {
		struct { Operand left, right; } assign;
		struct { Operand result, op1, op2; } binop;
	};
} InterCode;

typedef struct InterCodeNode {
	InterCode code;
	struct InterCodeNode *prev, *next;
} InterCodeNode;
/*
Note that every empty InterCodeNode shall be initialized:
	InterCodeNode icn;
	icn.next = &icn, icn.prev = &icn;
*/

extern const struct Operand NULLOP;
extern InterCodeNode InterCodeHead;
extern int InterCodeVarCnt;

void initIR();
Operand generateTemp();
InterCodeNode *InterCodeCat(int cnt, ...);
InterCodeNode *InterCodeAppend(InterCodeNode *head, InterCode code);
void deleteInterCode();
void printInterCode();

#endif