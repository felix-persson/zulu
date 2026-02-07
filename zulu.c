#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "parser.tab.h"


extern FILE* yyin;

extern int yylex(void);
extern int yyerror(const char* s);

extern node* program;

int
main(int argc, char** argv)
{
	if (argc == 2) {
		yyin = fopen(argv[1], "r");
		if (!yyin) {
			fprintf(stderr, "Couldn't open source file. Using stdin.\n");
			exit(1);
	//		do {
	//			yyparse();
	//		} while(!feof(yyin));
		}
	}

	yyparse();

	if (!program) {
		fprintf(stderr, "Parsing failed\n");
		exit(1);
	}

	FILE* tree_f = fopen("tree.dot", "w");

	if (!tree_f) {
		fprintf(stderr, "Couldn't open tree file.\n");
		exit(1);
	}

	generate_tree(program, tree_f);
	fclose(tree_f);

	puts("\nBuilt a parse-tree at ./tree.dot. Use \"make tree\" to generate \
the pdf version.");


	return EXIT_SUCCESS;
}
