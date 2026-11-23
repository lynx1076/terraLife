#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>


int ParseLong(long* value, char* str, size_t length);
int ParseFloat(float* value, const char* str, size_t length);
long FloorDiv(long a, long b);

#endif
