#include <string.h>
#include "ast.h"
#include "util.h"

static node*
nalloc(node_t type, const char* value, int line, size_t capacity)
{
	node* n = malloc(sizeof(node) + capacity * sizeof(node*));
	if (!n) {
		perror("malloc");
		exit(1);
	}
	memset(n->children, 0, capacity * sizeof(node*));

    n->type     = type;
    n->line     = line;
    n->value    = value ? strdup(value) : NULL;
    n->is_array = false;
    n->capacity = capacity;
    n->count    = capacity; /* may be overridden */

    return n;
}

static int
_generate_tree(node* n, int count, FILE* f)
{
	int id = count++;
	if (n->value == NULL) {
		fprintf(f, "n%d [label=\"%s\"];\n", id, get_type(n->type));
	}
	else {
		if (n->is_array) {
			fprintf(f, "n%d [label=\"ARRAY_%s:%s\"];\n",
					id, get_type(n->type), n->value);
		}
		else {
			fprintf(f, "n%d [label=\"%s:%s\"];\n",
					id, get_type(n->type), n->value);
		}
	}

	for (size_t i = 0; i < n->count; ++i) {
		if (n->children[i] != NULL) {
			int child_id = count;
			count = _generate_tree(n->children[i], count, f);
			fprintf(f, "n%d -> n%d\n", id, child_id);
		}
	}

	return count;
}

void
generate_tree(node* root, FILE* f)
{
	fputs("digraph{\n", f);
	_generate_tree(root, 0, f);
	fputs("\n}", f);
}

node*
make_group(node_t type)
{
	node* n = nalloc(type, NULL, -1, 4); /* start size = 4 */
	n->count = 0;

    return n;
}

node*
group_add(node* n, node* child)
{
	if (n->count == n->capacity) {
		n->capacity *= 2;
		n = realloc(n, sizeof(*n) + n->capacity * sizeof(n->children[0]));
		if (!n) {
			perror("Error when creating node");
			exit(1);
		}
	}
	n->children[n->count++] = child;

	return n;
}

node*
make_class_node(node* members, char* name, int line)
{
	node* n = nalloc(NODE_CLASS, name, line, 1);
	n->children[0] = members;

    return n;
}

node*
make_if_node(node* _if, node* block, node* _else, int line)
{
	node* n = nalloc(NODE_IF, NULL, line, 3);
	n->children[0] = _if;
	n->children[1] = block;
	n->children[2] = _else;

    return n;
}

node*
make_for_node(node* init, node* cond, node* change, node* stmts, int line)
{
	node* n = nalloc(NODE_FOR, NULL, line, 4);
	n->children[0] = init;
	n->children[1] = cond;
	n->children[2] = change;
	n->children[3] = stmts;

    return n;
}

node*
make_call_node(node* call_node, node* args, int line)
{
	node* n = nalloc(NODE_CALL, NULL, line, 2);
	n->children[0] = call_node;
	n->children[1] = args;

    return n;
}

node*
make_var_node(node* type, char* value, bool mutable, int line)
{
	node* n = nalloc(mutable ? NODE_VOLATILE_VAR : NODE_VAR, value, line, 1);
	n->children[0] = type;

    return n;
}

node*
make_vnode(node_t type, char* value, int line)
{
	node* n = nalloc(type, value, line, 0);
    return n;
}

node*
make_bnode(node_t type, node* l, node* r, int line)
{
	node* n = nalloc(type, NULL, line, 2);
	n->children[0] = l;
	n->children[1] = r;

	return n;
}

node*
make_unode(node_t type, node* operand, int line)
{
	node* n = nalloc(type, NULL, line, 1);
	n->children[0] = operand;

	return n;
}

const char*
get_type(node_t t)
{
	switch (t) {
	case NODE_TYPE:				return "TYPE";
	case NODE_INT:				return "INT";
	case NODE_FLOAT:			return "FLOAT";
	case NODE_BOOL:				return "BOOL";
	case NODE_ID:				return "ID";
	case NODE_ADD:				return "ADD";
	case NODE_SUB:				return "SUB";
	case NODE_MUL:				return "MUL";
	case NODE_DIV:				return "DIV";
	case NODE_POW:				return "POW";
	case NODE_GT:				return "GT";
	case NODE_LT:				return "LT";
	case NODE_GEQ:				return "GEQ";
	case NODE_LEQ:				return "LEQ";
	case NODE_EQ:				return "EQ";
	case NODE_NEQ:				return "NEQ";
	case NODE_AND:				return "AND";
	case NODE_OR:				return "OR";
	case NODE_VAR:				return "VAR";
	case NODE_VOLATILE_VAR:		return "VOLATILE_VAR";
	case NODE_ASSIGN:			return "ASSIGN";
	case NODE_NOT:				return "NOT";
	case NODE_PRINT:			return "PRINT";
	case NODE_READ:				return "READ";
	case NODE_RETURN:			return "RETURN";
	case NODE_GROUP:			return "GROUP";
	case NODE_COND:				return "COND";
	case NODE_IF:				return "IF";
	case NODE_ELSE:				return "ELSE";
	case NODE_METHOD:			return "METHOD";
	case NODE_METHOD_GROUP:		return "METHOD_GROUP";
	case NODE_METHOD_BODY:		return "METHOD_BODY";
	case NODE_MEMBERS:			return "MEMBERS";
	case NODE_MAIN:				return "MAIN";
	case NODE_CLASS:			return "CLASS";
	case NODE_CLASS_GROUP:		return "CLASS_GROUP";
	case NODE_PROGRAM_GROUP:	return "PROGRAM_GROUP";
	case NODE_STMT_GROUP:		return "STMT_GROUP";
	case NODE_VAR_GROUP:		return "VAR_GROUP";
	case NODE_PARAM_GROUP:		return "PARAM_GROUP";
	case NODE_BREAK:			return "BREAK";
	case NODE_CONTINUE:			return "CONTINUE";
	case NODE_VOID:				return "VOID";
	case NODE_FOR:				return "FOR";
	case NODE_LENGTH:			return "LENGTH";
	case NODE_CALL:				return "CALL";
	case NODE_DOT:				return "DOT";
	case NODE_ARRAY:			return "ARRAY";
	case NODE_ARRAY_INDEX:		return "ARRAY_INDEX";
	case NODE_ARG_GROUP:		return "ARG_GROUP";
	case NODE_POSTFIX:			return "POSTFIX";
	default:					return "?";
	}
}
