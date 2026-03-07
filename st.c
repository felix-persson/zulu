#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "st.h"
#include "util.h"

static void st_program_gen_syms(st*, node*);
static void st_class_gen_syms(st*, node*);
//static void st_method_gen_syms(st*, node*);
static void st_var_gen_syms(st*, node*);
//static void st_stmt_gen_syms(st*, node*);
//static void st_exp_gen_syms(st*, node*);
static void st_main_gen_syms(st*, node*);
static st* st_init(void);

static st*
st_init(void)
{
	st* t = malloc(sizeof(st));

	if (!t) {
		fprintf(stderr, "Error when initializing symbol table\n");
		exit(1);
	}

	for (int i = 0; i < ST_HASH_SIZE; i++) t->table[i] = NULL;
	
	t->parent = NULL;
	return t;
}

st*
st_enter_scope(st* s)
{
	st* t = st_init();
	t->parent = s;
	printf("[ST] Changed scope: %p -> %p\n", (void*)s, (void*)t);
	return t;
}

st*
st_exit_scope(st* t)
{
	printf("[ST] Changed scope: %p -> %p\n", (void*)t, (void*)t->parent);
	return t->parent;
}

static void
st_program_gen_syms(st* t, node* root)
{
	if (root->count != 3) {
		fprintf(stderr, "Invalid AST\n");
		exit(1);
	}

	st_var_gen_syms(t, root->children[0]);
	puts("[ST] Generated symbols for variable declarations");
	st_class_gen_syms(t, root->children[1]);
	puts("[ST] Generated symbols for class declarations");
	st_main_gen_syms(t, root->children[2]);
	puts("[ST] Generated symbols for main method");
}

static void
st_class_gen_syms(st* t, node* n)
{
	if (n->count == 0) {
		puts("No classes; skipping");
		return;
	}

	size_t i = 0;
	node* cn;
	for (cn = n->children[i]; i < n->count; cn = n->children[i++]) {

		/* ENTER SCOPE */
		st* subt = st_enter_scope(t);

		printf("Class %ld\n", i);
		if (cn->children[0]->count == 0) {
			printf("Class %s is empty\n", cn->value);
		}

		/* EXIT SCOPE */

		free(subt);
	}
}

//static void
//st_method_gen_syms(st* t, node* method)
//{
//	puts("Generating method symbols");
//}

//static void
//st_stmt_gen_syms(st* t, node* stmt)
//{
//	puts("Generating statement symbols");
//}

static void
st_var_gen_syms(st* t, node* n)
{
	if (n->count == 0) {
		puts("No variables; skipping");
		return;
	}
}

//static void
//st_exp_gen_syms(st* t, node* exp)
//{
//	puts("Generating expression symbols");
//}

static void
st_main_gen_syms(st* t, node* main_node)
{
	puts("[ST] Generating method symbols");
	st* main_scope = st_enter_scope(t);
	node* stmt_group = main_node->children[0];

	for (size_t i = 0; i < stmt_group->count; i++) {
		printf("[ST] 	Found statement (%ld)\n", i);
		node* stmt = stmt_group->children[i];
		sym* s = NULL;

		switch (stmt->type) {
		case NODE_VAR:
		case NODE_ASSIGN:
			puts("First type");
			s = st_put_sym(main_scope, stmt->value, SYM_VAR, stmt->line);
			if (s) s->node = stmt;
			break;
		case NODE_PRINT:
		case NODE_READ:
		case NODE_RETURN:
			puts("Second type");
			s = st_put_sym(main_scope, stmt->value, SYM_BUILTIN,
								stmt->line);
			if (s) s->node = stmt;
			break;
			break;
		default:
			fprintf(stderr, "Unknown node.\n");
			exit(1);
		}

	}

	st_exit_scope(main_scope);
}

sym*
st_put_sym(st* t, char* name, sym_t type, int line)
{
	if (!name) { name = ""; }
	int idx = hash(name, ST_HASH_SIZE);
	printf("Symbol name %s has hash %d\n", name, idx);

	for (sym* s = t->table[idx]; s; s = s->next) {
		if (strcmp(s->name, name) == 0) {
			fprintf(stderr,
				"[ST] Error on l. %d: '%s' is already declared in"
				"this scope on l. %d\n",
				line, name, s->line
			);
			exit(1);
		}
	}

	sym* s = malloc(sizeof(sym));
	if (!s) {
		perror("malloc");
		exit(1);
	}

	s->name = name;
	s->type = type;
	s->next = t->table[idx];
	s->line = line;

	t->table[idx] = s;
	return s;
}

sym*
st_get_sym(st* t, char* name)
{
	int idx = hash(name, ST_HASH_SIZE);

	for (st* scope = t; scope != NULL; scope = scope->parent) {
		for (sym* s = scope->table[idx]; s; s = s->next) {
			if(strcmp(s->name, name) == 0) {
				return s;
			}
		}
	}
	
	if (t->parent == NULL) {
		return NULL;
	}
	
	return st_get_sym(t->parent, name);
}

void
st_print(st* t)
{
    if (!t) return;

    printf("┌── Scope (level %d, addr %p) ─────────────────\n",
           t->scope_level, (void*)t);

    int found = 0;
    for (int i = 0; i < ST_HASH_SIZE; i++) {
        for (sym* s = t->table[i]; s; s = s->next) {
            printf("│  %-20s   line=%d\n",
                   s->name, s->line);
            found++;
        }
    }
    if (!found) puts("│  (empty)");
    puts("└──────────────────────────────────────────────");
}

st*
st_gen(node* root)
{
	st* global_scope = st_init();
	st_program_gen_syms(global_scope, root);
	return global_scope;
}

