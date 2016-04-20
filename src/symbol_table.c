#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symbol_table.h"

unsigned int hash(char *name)
{
	unsigned int val = 0, i;
	for (; *name; name++) {
		val = (val << 2) + *name;
		if (i = val & ~HASH_MASK)
			val = (val ^ (i >> 12)) & HASH_MASK;
	}
	return val;
}

