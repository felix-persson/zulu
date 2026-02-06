VERSION = 1.0

PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

CFLAGS = -g -std=c11 -pedantic -Wall -Werror -O0
#CFLAGS = -std=c99 -pedantic -Wall -Werror -Wno-deprecated-declarations -O2

CC = clang
LEX = flex
YACC = bison -d
