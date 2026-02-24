#ifndef NODE_H
#define NODE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define LIST_BUFFER_SIZE 64

typedef enum {
	NODE_UNINIT,
	NODE_TYPE,
	NODE_INT,
	NODE_FLOAT,
	NODE_BOOL,
	NODE_ID,
	NODE_ADD,
	NODE_SUB,
	NODE_MUL,
	NODE_DIV,
	NODE_POW,
	NODE_GT,
	NODE_LT,
	NODE_GEQ,
	NODE_LEQ,
	NODE_EQ,
	NODE_NEQ,
	NODE_AND,
	NODE_OR,
	NODE_VAR,
	NODE_ASSIGN,
	NODE_NOT,
	NODE_PRINT,
	NODE_READ,
	NODE_RETURN,
	NODE_GROUP,
	NODE_COND,
	NODE_IF,
	NODE_ELSE,
	NODE_METHOD,
	NODE_METHOD_GROUP,
	NODE_METHOD_BODY,
	NODE_MEMBERS,
	NODE_MAIN,
	NODE_CLASS,
	NODE_CLASS_GROUP,
	NODE_PROGRAM_GROUP,
	NODE_STMT_GROUP,
	NODE_VAR_GROUP,
	NODE_PARAM_GROUP,
	NODE_BREAK,
	NODE_CONTINUE,
	NODE_VOID,
	NODE_FOR,
	NODE_LENGTH,
	NODE_CALL,
	NODE_DOT,
	NODE_ARRAY,
	NODE_ARRAY_INDEX,
	NODE_ARG_GROUP,
	NODE_POSTFIX,
} node_t;

typedef struct node node;

struct node {
	node_t type;
	int line, id;
	char* value;
	bool is_array;
	size_t capacity, count;
	node* children[];
};

node* make_group(node_t);
node* group_add(node*, node*);

node* make_vnode(node_t, char*, int);
node* make_bnode(node_t, node*, node*, int);
node* make_unode(node_t, node*, int);
node* make_var_node(node*, char*, int);
node* make_class_node(node*, char*, int);
node* make_type_node(node_t, char*, int);
node* make_if_node(node*, node*, node*, int);
node* make_for_node(node*, node*, node*, node*, int);
node* make_call_node(node*, node*, int);

void generate_tree(node*, FILE*);

char *get_type(node_t);

#endif
