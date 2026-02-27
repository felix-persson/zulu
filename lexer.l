%{
#include <stdio.h>
#include "node.h"
#include "util.h"
#include "parser.tab.h"
%}

%option yylineno noyywrap nounput batch noinput nostack
%option never-interactive

%%

^[[:space:]]*\r?\n { /* empty lines; ignore */ }
"//"[^\n]*	{ /* comments; ignore */ }

	/* line termination */
\n+		return NL;

	/* everything else */
[ \t\r]+	{ /* whitespace; ignore */}

	/* operators */
"+"			return yytext[0];
"-"			return yytext[0];
"*"			return yytext[0];
"/"			return yytext[0];
"^"			return yytext[0];
">="		return GEQ;
"<="		return LEQ;
">"			return yytext[0];
"<"			return yytext[0];
"="			return yytext[0];
"!="		return NEQ;
"&"			return yytext[0];
"|"			return yytext[0];
":="		return ASSIGN;
"!"			return yytext[0];

	/* keywords */
"if"		return IF;
"else"		return ELSE;
"for"		return FOR;
"print"		return PRINT;
"read"		return READ;
"return"	return RETURN;
"break"		return BREAK;
"continue"	return CONTINUE;
"class"		return CLASS;
"main"		return MAIN;
"length"	return LENGTH;

	/* delimiters */
"("			return yytext[0];
")"			return yytext[0];
"["			return yytext[0];
"]"			return yytext[0];
"{"			return yytext[0];
"}"			return yytext[0];
"."			return yytext[0];
","			return yytext[0];
":"			return yytext[0];
";"			return yytext[0];

	/* types */
"int"		{ yylval.s = strdup(yytext); return INT_T; }
"float"		{ yylval.s = strdup(yytext); return FLOAT_T; }
"boolean"	{ yylval.s = strdup(yytext); return BOOL_T; }
"void"		{ yylval.s = strdup(yytext); return VOID_T; }

	/* qualifiers */
"volatile"	return VOLATILE;

	/* literals */
"true"|"false"	{ yylval.s = strdup(yytext); return BOOL_L; }
0|[1-9][0-9]*	{ yylval.s = strdup(yytext); return INT_L; }
[0-9]+\.[0-9]+	{ yylval.s = strdup(yytext); return FLOAT_L; }

	/* identifiers */
[a-zA-Z_][a-zA-Z_0-9]*	{ yylval.s = strdup(yytext); return ID; }

<<EOF>>		return END;
.			{ printf("Unrecognized token %c\n", *yytext); }

%%
