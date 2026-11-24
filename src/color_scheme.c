#include "color_scheme.h"
#include <raylib.h>


#define X(NAME, R, G, B, A) \
  Color NAME = (Color){(R), (G), (B), (A)};

COLORS

#undef X

