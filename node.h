#ifndef NODE_H
#define NODE_H

typedef struct node node;

struct node {
	char type;
	union {
		struct {
			node* left;
			node* right;
		};
		struct {
			size_t capacity;
			size_t count;
			node** nodes;
		};
		int i_value;
	};
};

#endif
