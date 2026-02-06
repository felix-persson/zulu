%{

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

extern int yylex(void);
extern int yyparse(void);
void yyerror(const char* s);

%}

%union {
	node* n;
	float f;
	int   i;
	char* s;
}

%start root

%token ADD SUB MUL DIV POW
%token GEQ LEQ GT LT EQ NEQ AND OR NOT ASSIGN
%token IF ELSE FOR PRINT READ RETURN BREAK CONTINUE CLASS
%token L_PAR R_PAR L_BRACKET R_BRACKET L_BRACE R_BRACE
%token PERIOD COMMA COLON SEMICOLON
%token VOLATILE
%token INT_LIT FLOAT_LIT BOOL_LIT STRING_LIT
%token ID
%token<i> INT
%token<f> FLOAT
%token<i> BOOL
%token VOID

%type<n> exp val

%left ADD SUB
%left MUL DIV

%%

root
	: exp { }
	;

exp
	: val { $$ = $1; }
	;

val
	: INT_LIT { }
	;

%%

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
