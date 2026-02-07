#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "node.h"
#include "util.h"

node*
make_group(node_t type)
{
	size_t start_size = 4;

	node* l = malloc(sizeof(*l) + sizeof(l->children[0]) * start_size);

    if (!l) {
        perror("Error when creating node");
        exit(1);
    }

	l->capacity = start_size;
	l->count = 0;
    l->node_type = type;

    return l;
}


/* group_add returns the same list node in case of reallocation. */
node*
group_add(node* l, node* child)
{
	if (l->count == l->capacity) {
		l->capacity *= 2;
		l = realloc(l, sizeof(*l) + l->capacity * sizeof(l->children[0]));
	}
	l->children[l->count++] = child;

	return l;
}


node*
make_type_node(node_t type, char* value, int line)
{
    node* new = malloc(sizeof(node));
    if(!new) {
        perror("Error when creating node");
        exit(1);
    }

    new->node_type = NODE_TYPE;
	new->line = line;
    new->value = strdup(value);

    return new;
}

static void
_eval_node(node* n, int* count, FILE* f)
{
	n->id = (*count)++;
	if (n->value == NULL)
		fprintf(f, "n%d [label=\"%s\"];\n", n->id, get_type(n->node_type));
	else
		fprintf(f, "n%d [label=\"%s:%s\"];\n", n->id, get_type(n->node_type), n->value);

	for (int i = 0; i < n->count; ++i) {
		if (n->children[i] != NULL) {
			_eval_node(n->children[i], count, f);
			fprintf(f, "n%d -> n%d\n", n->id, n->children[i]->id);
		}
	}
}

void
generate_tree(node* root, FILE* f)
{
	int count = 0;

	fputs("digraph{\n", f);
	_eval_node(root, &count, f);
	fputs("\n}", f);
}

node*
make_class_node(node* members, char* name, int line)
{
	size_t child_count = 1;
    node* new = malloc(sizeof(*new) + child_count * sizeof(new->children[0]));

    if(!new) {
        perror("Error when creating node");
        exit(1);
    }
    new->node_type = NODE_CLASS;
	new->value = name;
	new->line = line;
	new->capacity = child_count;
	new->count = child_count;
	new->children[0] = members;

    return new;
}
node*
make_var_node(node* type, char* value, int line)
{
    node* new = malloc(sizeof(node));
    if(!new) {
        perror("Error when creating node");
        exit(1);
    }

    new->node_type = NODE_VAR;
	new->line = line;
    new->value = strdup(value);
	new->capacity = 0;

    return new;
}


node*
make_value_node(node_t type, char* value, int line)
{
    node* new = malloc(sizeof(node));
    if(!new) {
        perror("Error when creating node");
        exit(1);
    }
    new->node_type = type;
	new->line = line;
    new->value = strdup(value);
	new->capacity = 0;

    return new;
}

node*
make_bnode(node_t type, node* lhs, node* rhs, int line)
{
	size_t child_count = 2;
    node* new = malloc(sizeof(*new) + child_count * sizeof(new->children[0]));

    if(!new) {
        perror("Error when creating node");
        exit(1);
    }
    new->node_type = type;
	new->line = line;
	new->capacity = child_count;
	new->count = child_count;
	new->children[0] = lhs;
	new->children[1] = rhs;

    return new;
}

node*
make_unode(node_t type, node* operand, int line)
{
	size_t child_count = 1;
    node* new = malloc(sizeof(*new) + child_count * sizeof(new->children[0]));

    if(!new) {
        perror("Error when creating node");
        exit(1);
    }
    new->node_type = type;
	new->line = line;
	new->capacity = child_count;
	new->count = child_count;
	new->children[0] = operand;

    return new;
}

node*
make_if_node(node* cond, node* block, node* _else, int line)
{
	size_t child_count = 3;
    node* new = malloc(sizeof(*new) + child_count * sizeof(new->children[0]));

    if(!new) {
        perror("Error when creating node");
        exit(1);
    }
    new->node_type = NODE_IF;
	new->line = line;
	new->capacity = child_count;
	new->count = child_count;
	new->children[0] = cond;
	new->children[1] = block;
	new->children[2] = _else == NULL ? NULL : _else;

    return new;
}

node*
make_for_node(node* init, node* cond, node* change, node* stmts, int line)
{
	size_t child_count = 4;
    node* new = malloc(sizeof(*new) + child_count * sizeof(new->children[0]));

    if(!new) {
        perror("Error when creating node");
        exit(1);
    }
    new->node_type = NODE_FOR;
	new->line = line;
	new->capacity = child_count;
	new->count = child_count;
	new->children[0] = init;
	new->children[1] = cond;
	new->children[2] = change;
	new->children[3] = stmts;

    return new;
}

node*
make_call_node(node* call_node, node* args, int line)
{
	size_t child_count = 2;
    node* new = malloc(sizeof(*new) + child_count * sizeof(new->children[0]));

    if(!new) {
        perror("Error when creating node");
        exit(1);
    }
    new->node_type = NODE_CALL;
	new->line = line;
	new->capacity = child_count;
	new->count = child_count;
	new->children[0] = call_node;
	new->children[1] = args;

    return new;
}

char*
get_type(node_t t)
{
	switch (t) {
	case NODE_TYPE: 		return "TYPE"; puts("!");
	case NODE_INT:			return "INT";
	case NODE_FLOAT:		return "FLOAT";
	case NODE_BOOL:			return "BOOLEAN";
	case NODE_ID:			return "ID";
	case NODE_ADD:			return "ADD";
	case NODE_SUB:			return "SUB";
	case NODE_MUL:			return "MUL";
	case NODE_DIV:			return "DIV";
	case NODE_POW:			return "POW";
	case NODE_GT:			return "GT";
	case NODE_LT:			return "LT";
	case NODE_GEQ:			return "GEQ";
	case NODE_LEQ:			return "LEQ";
	case NODE_EQ:			return "EQ";
	case NODE_AND:			return "AND";
	case NODE_OR:			return "OR";
	case NODE_VAR:			return "VAR";
	case NODE_ASSIGN:		return "ASSIGN";
	case NODE_NOT:			return "NOT";
	case NODE_PRINT:		return "PRINT";
	case NODE_READ:			return "READ";
	case NODE_RETURN:		return "RETURN";
	case NODE_GROUP:		return "GROUP";
	case NODE_COND:			return "COND";
	case NODE_METHOD:		return "METHOD";
	case NODE_METHOD_GROUP:	return "METHOD_GROUP";
	case NODE_METHOD_BODY:	return "METHOD_BODY";
	case NODE_MEMBERS: 		return "MEMBERS";
	case NODE_MAIN: 		return "MAIN";
	case NODE_CLASS: 		return "CLASS";
	case NODE_CLASS_GROUP: 	return "CLASS_GROUP";
	case NODE_STMT_GROUP: 	return "STMT_GROUP";
	case NODE_PROGRAM_GROUP: return "PROGRAM_GROUP";
	case NODE_VAR_GROUP: 	return "VAR_GROUP";
	case NODE_PARAM_GROUP: 	return "PARAM_GROUP";
	case NODE_FOR: 			return "FOR_LOOP";
	case NODE_IF:			return "COND_IF";
	case NODE_CALL:			return "CALL";
	case NODE_DOT:			return "DOT";
	case NODE_LENGTH:		return "LENGTH";
	case NODE_ARRAY:		return "ARRAY";
	case NODE_ARRAY_INDEX:	return "ARRAY_INDEX";
	case NODE_ARG_GROUP:	return "ARG_GROUP";
	default:				return "?";
	}
}

