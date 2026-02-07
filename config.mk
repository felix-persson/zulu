VERSION = 1.0

CFLAGS = -g -std=c99 -pedantic -Wall -Werror -O0
#CFLAGS = -std=c99 -pedantic -Wall -Werror -Wno-deprecated-declarations -Os

CC = clang
LEX = flex
YACC = bison -d -Wcounterexamples
