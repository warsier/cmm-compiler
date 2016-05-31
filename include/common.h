#ifndef __COMMON_H__
#define __COMMON_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

typedef unsigned bool;
#define true 1
#define false 0

#define STREQ(x, y) (strcmp((x), (y)) == 0)

#define MAX_LEN 30 // the length of symbol and name shall not exceed MAX_LEN

extern bool errorStat;

#endif