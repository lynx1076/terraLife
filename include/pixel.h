#ifndef PIXEL_H
#define PIXEL_H

#include <sys/types.h>

typedef enum {
  pix_Died=-1,
  pix_Dead=0,
  pix_Alive=2,
  pix_Born=1,
} PixelState;

typedef struct {
  ssize_t x;
  ssize_t y;
} PixelCoords;

#endif
