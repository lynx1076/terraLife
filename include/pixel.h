#ifndef PIXEL_H
#define PIXEL_H

#include <raylib.h>
#include <stddef.h>
#include <stdint.h>

#define PIXEL_ARENA_SIZE          256
#define PIXEL_SCALE               20

#define PIXEL_DEAD    0
#define PIXEL_DIED    1
#define PIXEL_ALIVE   2
#define PIXEL_BORN    3


typedef struct {
  size_t x;
  size_t y;
} PixelCoords;

typedef uint8_t PixelState;

typedef struct {
  uint8_t pixels[PIXEL_ARENA_SIZE][PIXEL_ARENA_SIZE];
  Texture2D tex;
} PixelArenaData;

typedef struct {
  PixelCoords pos;
  PixelArenaData* data;
} PixelArena;


int pix_PixelArenaInit(PixelArena* pxa, PixelCoords pos);
void pix_PixelArenaFree(PixelArena* pxa);
int pix_PixelArenaFill(PixelArena* pxa, PixelState fill);
int pix_PixelArenaDraw(PixelArena* pxa);
int pix_PixelArenaSetPix(PixelArena* pxa, unsigned int x, unsigned int y, PixelState state);
int pix_PixelArenaStep(PixelArena* pxa);
PixelCoords pix_GlobalToArenaCoords(PixelCoords pos);

#endif
