#pragma once
#pragma once
#include <stdlib.h>
#include <assert.h>

#pragma warning(disable:4996)

namespace Stack
{
	struct Stack
	{
		char** data;
		int n_data;
		int size;
	};

	void resize(Stack *c)
	{
		c->size += c->size >> 1;
		c->data = (char**)realloc(c->data, sizeof(char)*c->size); assert(c->data);
	}

	void init(Stack* c, unsigned int initial_size)
	{
		assert(initial_size != 0);
		c->n_data = 0;
		c->size = initial_size;
		c->data = (char**)malloc(sizeof(char)*c->size); assert(c->data);
	}

	void push(Stack *c, char* val)
	{
		if (c->n_data >= c->size) resize(c);
		c->data[c->n_data++] = val;
	}

	char* pop(Stack *c)
	{
		return c->data[--c->n_data];
	}

	void clear(Stack *c)
	{
		c->n_data = 0;
	}

	void cleanup(Stack *c)
	{
		free(c->data);
		*c = {};
	}
}