#include "util.h"
#include <stdlib.h>
#include <string.h>

char*
strdup(const char* src)
{
	char* dst = malloc(strlen(src) + 1);
	if (dst == NULL)
		return NULL;
	strcpy(dst, src);
	return dst;
}

int
hash(char* str, int mod)
{
	unsigned int hash = 0;
	while (*str) {
		hash = (hash << 1) + *str++;
	}
	return hash % mod;
}
