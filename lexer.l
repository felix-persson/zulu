%{
#include<stdio.h>
#include "node.h"
#include "parser.tab.h"
%}

%option noyywrap nounput batch noinput
%option never-interactive

%%
	/* operators */
"+"				return ADD;
"-"				return SUB;
"*"				return MUL;
"/"				return DIV;
"^"				return POW;
">="			return GEQ;
"<="			return LEQ;
">"				return GT;
"<"				return LT;
"="				return EQ;
"!="			return NEQ;
"&"				return AND;
"|"				return OR;
":="			return ASSIGN;
"!"				return NOT;

	/* keywords */
"if"			return IF;
"else"			return ELSE;
"for"			return FOR;
"print"			return PRINT;
"read"			return READ;
"return"		return RETURN;
"break"			return BREAK;
"continue"		return CONTINUE;
"class"			return CLASS;

	/* delimiters */
"("             return L_PAR;
")"             return R_PAR;
"["             return L_BRACKET;
"]"             return R_BRACKET;
"{"             return L_BRACE;
"}"             return R_BRACE;
"."             return PERIOD;
","             return COMMA;
":"             return COLON;
";"             return SEMICOLON;

	/* types */
"int"			return INT;
"float"			return FLOAT;
"boolean"		return BOOL;
"void"			return VOID;

	/* qualifiers */
"volatile"		return VOLATILE;

	/* literals */
"true"|"false"			{ yylval.i = (*yytext == 't'); return BOOL_LIT;}
[a-zA-Z_][a-zA-Z_0-9]*	{ yylval.s = yytext; return ID; }
0|[1-9][0-9]*			{ yylval.i = atoi(yytext); return INT_LIT; }


	/* everything else */
[ \t\n\r]+		{}
"//"[^\n]*		{}
.				{ printf("Urecognized token %c\n", *yytext); }
<<EOF>>			return 0;

%%
