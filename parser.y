%{

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

extern int yylineno;
extern int yylex(void);
extern int yyparse(void);
void yyerror(const char* s);
node* program;

%}

%union {
	bool  b;
	float f;
	int   i;
	node* n;
	char* s;
}

%token GEQ LEQ NEQ ASSIGN
%token PRINT READ RETURN BREAK CONTINUE CLASS MAIN LENGTH IF ELSE FOR
%token INT_T FLOAT_T BOOL_T
%token<s> ID INT_L FLOAT_L BOOL_L VOID_T VOLATILE
%token NL

%type<s> primitive
%type<n> program type exp exps stmt stmts var vars_opt var_decl var_decls var_decls_opt
%type<n> cond method main block class_decl class_decls_opt class_member
%type<n> class_members class_members_opt cond_body postfix literal
%type<n> for for_arg1 for_arg2 assign

%right ASSIGN
%left '|'
%left '&'
%left '<' '>' LEQ GEQ
%left '=' NEQ
%left '+' '-'
%left '*' '/'
%left '^'
%left '!'
%left '.' '[' ']'

%nonassoc _ELSE
%nonassoc ELSE 
%token END 0 "end of file"

%start program
%%

program
	: vars_opt class_decls_opt main NL END { $$ = make_group(NODE_PROGRAM_GROUP); $$ = group_add($$, $1); $$ = group_add($$, $2); $$ = group_add($$, $3); program = $$; }
	;

main
	: MAIN '(' ')' ':' INT_T block { $$ = make_unode(NODE_MAIN, $6, yylineno); }
	;

vars_opt
	: /* empty */ { $$ = make_group(NODE_VAR_GROUP); }
	| vars_opt var NL { $$ = group_add($1, $2); }
	;

class_decls_opt
	: /* empty */ { $$ = make_group(NODE_CLASS_GROUP); }
	| class_decls_opt class_decl NL { $$ = group_add($1, $2); }
	;

class_decl
	: CLASS ID '{' NL class_members_opt '}' { $$ = make_class_node($5, $2, yylineno); }
	;

class_members_opt
	: /* empty */ { $$ = make_group(NODE_MEMBERS); }
	| class_members { $$ = $1; }
	;

class_members
	: class_member NL { $$ = make_group(NODE_MEMBERS); $$ = group_add($$, $1); }
	| class_members class_member NL { $$ = group_add($1, $2); }
	;

class_member
	: var
	| method
	;

method
	: ID '(' var_decls_opt ')' ':' type block { $$ = make_group(NODE_METHOD); $$ = group_add($$, $3); $$ = group_add($$, $6); $$ = group_add($$, $7); }
	;

var_decls_opt
	: /* empty */ { $$ = make_group(NODE_PARAM_GROUP); }
	| var_decls { $$ = $1; }
	;

block
	: '{' NL stmts '}' { $$ = $3; }
	;

stmts
	: /* empty */ { $$ = make_group(NODE_STMT_GROUP); }
	| stmts NL { $$ = $1; }
	| stmts stmt { $$ = group_add($1, $2); }
	;

stmt
	: block
	| PRINT '(' exp ')' NL { $$ = make_unode(NODE_PRINT, $3, yylineno); }
	| READ '(' exp ')' NL { $$ = make_unode(NODE_READ, $3, yylineno); }
	| RETURN exp NL { $$ = make_unode(NODE_RETURN, $2, yylineno); }
	| BREAK NL { $$ = make_unode(NODE_BREAK, NULL, yylineno); }
	| CONTINUE NL { $$ = make_unode(NODE_CONTINUE, NULL, yylineno); }
	| exp ASSIGN exp NL { $$ = make_bnode(NODE_ASSIGN, $1, $3, yylineno); }
	| exp NL
	| var NL
	| cond
	| for
	;

for
	: FOR '(' for_arg1 ',' for_arg2 ',' assign ')' stmt
		{ $$ = make_for_node($3, $5, $7, $9, yylineno); }
	;

for_arg1
	: /* empty */ { $$ = NULL; }
	| var
	| assign
	;

for_arg2
	: /* empty */ { $$ = NULL; }
	| exp
	;

cond
	: IF '(' exp ')' cond_body %prec _ELSE { $$ = make_if_node($3, $5, NULL, yylineno); }
	| IF '(' exp ')' cond_body ELSE cond_body { $$ = make_if_node($3, $5, $7, yylineno); }
	;

cond_body
	: stmt
	;

var
	: VOLATILE ID ':' type { $$ = make_var_node($4, $2, yylineno); }
	| VOLATILE ID ':' type ASSIGN exp { $$ = make_bnode(NODE_ASSIGN, make_var_node($4, $2, yylineno), $6, yylineno); }
	| ID ':' type { $$ = make_var_node($3, $1, yylineno); }
	| ID ':' type ASSIGN exp { $$ = make_bnode(NODE_ASSIGN, make_var_node($3, $1, yylineno), $5, yylineno); }
	;

var_decls
	: var_decl { $$ = make_group(NODE_PARAM_GROUP); $$ = group_add($$, $1); }
	| var_decls ',' var_decl { $$ = group_add($1, $3); }
	;

var_decl
	: ID ':' type { $$ = make_var_node($3, $1, yylineno); }
	;

assign
	: exp ASSIGN exp { $$ = make_bnode(NODE_ASSIGN, $1, $3, yylineno); }
	;

exps
	: exp { $$ = make_group(NODE_GROUP); $$ = group_add($$, $1); }
	| exps ',' exp { $$ = group_add($1, $3); }
	;

exp
	: exp '&' exp { $$ = make_bnode(NODE_AND, $1, $3, yylineno); }
	| exp '|' exp { $$ = make_bnode(NODE_OR, $1, $3, yylineno); }
	| exp '<' exp { $$ = make_bnode(NODE_LT, $1, $3, yylineno); }
	| exp '>' exp { $$ = make_bnode(NODE_GT, $1, $3, yylineno); }
	| exp LEQ exp { $$ = make_bnode(NODE_LEQ, $1, $3, yylineno); }
	| exp GEQ exp { $$ = make_bnode(NODE_GEQ, $1, $3, yylineno); }
	| exp '=' exp { $$ = make_bnode(NODE_EQ, $1, $3, yylineno); }
	| exp NEQ exp { $$ = make_bnode(NODE_NEQ, $1, $3, yylineno); }
	| exp '+' exp { $$ = make_bnode(NODE_ADD, $1, $3, yylineno); }
	| exp '-' exp { $$ = make_bnode(NODE_SUB, $1, $3, yylineno); }
	| exp '*' exp { $$ = make_bnode(NODE_MUL, $1, $3, yylineno); }
	| exp '/' exp { $$ = make_bnode(NODE_DIV, $1, $3, yylineno); }
	| exp '^' exp { $$ = make_bnode(NODE_POW, $1, $3, yylineno); }
	| '!' exp     { $$ = make_unode(NODE_NOT, $2, yylineno); }
	| postfix
	| '(' exp ')' { $$ = $2; }
	;

literal
	: INT_L   { $$ = make_vnode(NODE_INT,   $1, yylineno); }
	| FLOAT_L { $$ = make_vnode(NODE_FLOAT, $1, yylineno); }
	| BOOL_L  { $$ = make_vnode(NODE_BOOL,  $1, yylineno); }
	| ID      { $$ = make_vnode(NODE_ID,    $1, yylineno); }
	;

postfix
	: literal
	| postfix '.' LENGTH { $$ = make_unode(NODE_LENGTH, $1, yylineno); }
	| postfix '(' exps ')' { $$ = make_call_node($1, $3, yylineno); $3->type = NODE_ARG_GROUP; }
	| postfix '(' ')' { $$ = make_call_node($1, NULL, yylineno); }
	| postfix '.' ID { node* _v = make_vnode(NODE_ID, $3, yylineno); $$ = make_bnode(NODE_DOT, $1, _v, yylineno); }
	| postfix '[' exps ']' { $$ = make_bnode(NODE_ARRAY_INDEX, $1, $3, yylineno); }
	;

type
	: ID { $$ = make_type_node(NODE_TYPE, $1, yylineno); }
	| VOID_T { $$ = make_type_node(NODE_VOID, $1, yylineno); }
	| primitive { $$ = make_type_node(NODE_TYPE, $1, yylineno); }
	| primitive '[' ']' { $$ = make_type_node(NODE_TYPE, $1, yylineno); $$->is_array = true; }
	;

primitive
	: INT_T { $$ = "INT_T"; }
	| FLOAT_T { $$ = "FLOAT_T"; }
	| BOOL_T { $$ = "BOOL_T"; }
	;

%%

void yyerror(const char* s) {
	fprintf(stderr, "Parse error at l. %d: %s\n", yylineno, s);
	exit(1);
}
