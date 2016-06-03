#include "calc.h"

int main(int argc, char *argv[])
{

				// Error-catching
				if (argc == 1) {
								printf("No expression provided!\n");
								return 1;
				}

				struct operatorStack oprStack;
				struct operandStack opdStack;
				oprStack.stack = malloc(sizeof(char)*25);
				oprStack.currentSize = 0;
				oprStack.capacity = 25;
				opdStack.stack = malloc(sizeof(char)*25);
				opdStack.currentSize = 0;
				opdStack.capacity = 25;
		 
				float result = calculate(argv[1], &oprStack, &opdStack);
		 
				printf("The result of:\t%s\nis:\t%f\n", argv[1], result);
				return 0;
}

float calculate(char *expr, struct operatorStack *rStack, struct operandStack *dStack)
{
				int closeParenIndex;
				int subresult;
				int end;
				int negDigit = 0;
						 
				// While we aren't at the end of the expression
				for (int i=0; expr[i] != '\0'; i++) {
								// If we find the open paren for a subexpr
								if (expr[i] == '(') {

												const char *closeParenPtr = strrchr(expr+i, ')'); // Find the last paren from current position (outermost one)
												if (closeParenPtr) { 
																closeParenIndex = closeParenPtr-expr; // Get the index in the overall expr
												} else { return -1; } // Didn't find paren; malformed expr.

												char *parenSubstr = malloc(sizeof(char)*(closeParenIndex-i));
												strncpy(parenSubstr, &expr[i+1], closeParenIndex-i);
												parenSubstr[closeParenIndex-i-1]='\0';

												pushOperator(rStack, 'e'); // Push e onto stack to initialize this "virtual stack"
												subresult = calculate(parenSubstr, rStack, dStack);
												popOperator(rStack); // Pop e off stack to close virtual stack
												if (i !=0 && expr[i-1] == '-') {
																pushOperand(dStack, (-1*subresult));
																negDigit = 0;
												} else {																										
																pushOperand(dStack, subresult);
												}
												i=closeParenIndex;							 
								} else {
												switch(expr[i]) {
												case '+':
												case '-':
																if (i == 0 || isOperator(expr[i-1])) {
																				negDigit = 1;
																				break;
																}
												case '^':
												case '/':
												case '*':
																if (cmpOp(peekOperator(rStack), expr[i]) == 1) {
																				subresult = doOperation(popOperator(rStack), popOperand(dStack), popOperand(dStack));
																				pushOperand(dStack, subresult);									
																}
																pushOperator(rStack, expr[i]);
																break;
												default: // Current char is a digit, may be part of a number
																end = i;
																while(isdigit(expr[end]) || expr[end] == '.') {
																				end++;
																}
																char *digitString = malloc(sizeof(char)*(end-i+1));
																strncpy(digitString, &expr[i], (end-i));
																digitString[end-i] = '\0';
																float num = atof(digitString);

																if (negDigit) {
																				pushOperand(dStack, (-1*num));
																				negDigit = 0;
																} else {
																				pushOperand(dStack, num);
																}
										
																i = end-1;
																free(digitString);										
												}
								}
				}
				// Found the end-of-expression, so process the stacks until we find an 'e', indicating the end of this call.
				// Each 'e' indicates the end of this 'virtual' stack. A final 'e' should be left on the stack when returning to main.
				while (peekOperator(rStack) != 'e') {
								pushOperand(dStack, doOperation(popOperator(rStack), popOperand(dStack), popOperand(dStack)));
				}
				return popOperand(dStack);
}

float doOperation(char operator, float leftOperand, float rightOperand)
{
				switch(operator) {
				case '+':
								return rightOperand+leftOperand;
				case '-':
								return rightOperand-leftOperand;
				case '*':
								return rightOperand*leftOperand;
				case '/':
								return rightOperand/leftOperand;
				case '^':
								return pow(rightOperand, leftOperand);
				default:
								return -1;
				}
}

int pushOperator(struct operatorStack *st, char opr)
{
				if ((*st).currentSize == (*st).capacity) {
								(*st).stack = realloc((*st).stack, sizeof(char)*((*st).capacity)*2);
								if ((*st).stack) { // Error-checking to make sure the realloc succeeded
												(*st).capacity *= 2;
								} else { return -1; }
				}
				(*st).stack[(*st).currentSize++] = opr;
				return 0;
}

char popOperator(struct operatorStack *st)
{
				if ((*st).currentSize == 0) {
								return 'e';
				} else {
								return (*st).stack[--(*st).currentSize];
				}
}

char peekOperator(struct operatorStack *st)
{
				if ((*st).currentSize == 0) {
								return 'e';
				} else {
								return (*st).stack[(*st).currentSize-1];
				}
}

int pushOperand(struct operandStack *st, float opd)
{
				if ((*st).currentSize == (*st).capacity) {
								(*st).stack = realloc((*st).stack, sizeof(float)*((*st).capacity)*2);
								if ((*st).stack) {
												(*st).capacity *= 2;
								} else { return -1; }
				}
				(*st).stack[(*st).currentSize++] = opd;
				return 0;
}

float popOperand(struct operandStack *st)
{
				if ((*st).currentSize == 0) {
								return 'e';
				} else {
								return (*st).stack[--(*st).currentSize];
				}
}

// Returns: 1 if leftOp>rightOp, 0 if same priority, -1 if leftOp<rightOp
int cmpOp(char leftOp, char rightOp)
{
				if (leftOp == 'e') { return -1; } // Empty stack is indicated by getting 'e'
				if (leftOp == rightOp) { return 0; }
					
				switch(leftOp) {
				case '^':
								return 1;
				case '*':
				case '/':
								if (rightOp == '-' || rightOp == '+') {
												return 1;
								} else if (rightOp == '^') {
												return -1;
								} else {
												return 0;
								}
				case '-':
				case '+':
								if (rightOp == '-' || rightOp == '+') {
												return 0;
								} else {
												return -1;
								}
				default:
								return 2;
				}
								 
}

int isOperator(char candidate)
{
				switch (candidate) {
				case '^':
				case '*':
				case '-':
				case '+':
				case '/':
								return 1;
				default:
								return 0;
				}
}
