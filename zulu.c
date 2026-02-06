#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "parser.tab.h"

extern FILE* yyin;

int yylex(void);
int yyerror(const char* s);


int
main(int argc, char** argv)
{
	yyin = stdin;
	do {
		yyparse();
	} while(!feof(yyin));

	return EXIT_SUCCESS;
}
