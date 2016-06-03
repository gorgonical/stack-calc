CC=clang
DEBUG=-g
LINKLIBS=-lm

all: calculator

calculator: calc.c
	$(CC) $(DEBUG) $(LINKLIBS) calc.c -o calculator
