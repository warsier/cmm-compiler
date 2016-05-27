#ifndef __IR_H__
#define __IR_H__

#include "syntax_tree.h"
#include "common.h"

typedef struct Operand {
	enum { VARIABLE, CONSTANT, ADDRESS } kind;
	union {
		int var_no;
		int value;
	};
} Operand;

typedef struct InterCode {
	enum { ASSIGN, ADD, SUB, MUL } kind;
	union {
		struct { Operand left, right; } assign;
		struct { Operand result, op1, op2; } binop;
	};
} InterCode;

typedef struct InterCodeNode {
	InterCode code;
	struct InterCodeNode *prev, *next;
} InterCodeNode;

extern InterCodeNode InterCodeHead;

void initIR();
InterCodeNode *InterCodeCat(int cnt, ...);
InterCodeNode *InterCodeAppend(InterCodeNode *head, InterCode code);
void deleteInterCode();
void printInterCode();

#endif