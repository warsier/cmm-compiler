#ifndef __COMMON_H__
#define __COMMON_H__

#include <string.h>

typedef unsigned bool;
#define true 1
#define false 0

#define STREQ(x, y) (strcmp((x), (y)) == 0)

#endif