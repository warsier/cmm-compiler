#ifndef __IR_H__
#define __IR_H__

#include "common.h"
#define OUTPUT_TO_SCREEN 1

#define INITICN(x) ((x).next = &(x), (x).prev = &(x))

typedef struct Operand {
	enum { VARIABLE, TEMP, LABEL, CONSTANT, ADDRESS } kind;
	union {
		int var_no;
		int value;
	};
} Operand;

/*
      kind  form
----------------------------------
LABEL_CODE  LABEL x :
  FUNCTION  FUNCTION f :
    ASSIGN  x := y
       ADD  x := y + z
       SUB  x := y - z
       MUL  x := y * z
      DIV_  x := y / z
            x := &y
            x := *y
            *x := y
LABEL_GOTO  GOTO x
LABEL_COND  IF x [relop] y GOTO z
    RETURN  RETURN x
       DEC  DEC x [size]
       ARG  ARG x
  CALLFUNC  x := CALL f
     PARAM  PARAM x
      READ  READ x
     WRITE  WRITE x
*/
typedef struct InterCode {
	enum { ASSIGN, ADD, SUB, MUL, DIV_, RETURN_, LABEL_CODE, LABEL_COND, LABEL_GOTO, READ, WRITE, CALL_FUNC, FUNCTION, ARG, PARAM, REFASSIGN, DEC } kind;
	union {
		struct { Operand left, right; } assign;
		struct { Operand result, op1, op2; } binop;
		Operand label_code;
		Operand label_goto;
		struct { Operand left, right, dest; char op[4]; } label_cond;
		struct  { Operand dest; char func[MAX_LEN]; } call_func;
		Operand arg;
		Operand read;
		Operand write;
		Operand return_;
		char function[MAX_LEN];
		Operand param;
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
Operand generateLabel();
InterCodeNode *InterCodeCat(int cnt, ...);
InterCodeNode *InterCodeAppend(InterCodeNode *head, InterCode code);
void deleteInterCode();
void printInterCode();

#endif