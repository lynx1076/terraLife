#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>


extern bool debug;

#ifdef DEBUG
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(EXPR) do { if (!(EXPR)) { printf("Assert failed on line %i of \"%s\": %s\n", __LINE__, __FILE__, #EXPR); exit(-1); } } while (0)
#else
#define ASSERT(EXPR) do {} while(0)
#endif

#endif
