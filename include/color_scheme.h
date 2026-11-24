#ifndef COLOR_SCHEME_H
#define COLOR_SCHEME_H

#include <raylib.h>

#define COLORS \
  X(color_bg, 20, 23, 31, 255) \
  X(color_highlight, 200, 200, 255, 100) \
  X(color_highlight2, 255, 255, 255, 80) \
  X(color_highlight_red, 255, 100, 155, 80) \
  X(color_highlight_yellow, 200, 200, 155, 80) \
  X(color_highlight_green, 0, 255, 0, 20) \
  X(color_born, 0x79, 0xAE, 0xA3, 255) \
  X(color_alive, 0x43, 0x43, 0x71, 255) \
  X(color_died, 0x3D, 0x00, 0x00, 255) \

#define X(NAME, R, G, B, A) \
  extern Color NAME;

COLORS

#undef X

#endif
