#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

struct operatorStack {
		 char *stack;
		 int currentSize;
		 int capacity;
};

struct operandStack {
		 float *stack;
		 int currentSize;
		 int capacity;
};

float calculate(char *expr, struct operatorStack *rStack, struct operandStack *dStack);
float doOperation(char operator, float leftOperand, float rightOperand);
int cmpOp(char leftOp, char rightOp);
float popOperand(struct operandStack *st);
int pushOperand(struct operandStack *st, float opd);
char peekOperator(struct operatorStack *st);
char popOperator(struct operatorStack *st);
int pushOperator(struct operatorStack *st, char opr);
int isOperator(char candidate);
