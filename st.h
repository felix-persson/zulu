#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define ST_HASH_SIZE 317

typedef struct sym sym;
typedef struct st st;

typedef enum {
	SYM_VAR,
	SYM_PARAM,
	SYM_CLASS,
	SYM_METHOD,
	SYM_FIELD,
	SYM_BUILTIN
} sym_t;

struct sym {
	char* name;
	sym_t type;
	int line;
	union {
		struct node* node;
	};
	sym* next;
};

struct st {
	sym* table[ST_HASH_SIZE];
	st* parent;
	int scope_level;
};

void st_print(st*);
st* st_gen(node*);
st* st_enter_scope(st*);
st* st_exit_scope(st*);
sym* st_put_sym(st*, char*, sym_t, int);
sym* st_get_sym(st*, char*);

#endif
