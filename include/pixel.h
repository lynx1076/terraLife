#ifndef PIXEL_H
#define PIXEL_H

#include <raylib.h>
#include <stddef.h>
#include <stdint.h>

#define PIXEL_ARENA_SIZE          256
#define PIXEL_SCALE               20

typedef struct {
  uint8_t pixels[PIXEL_ARENA_SIZE][PIXEL_ARENA_SIZE];
  Texture2D tex;
} PixelArenaData;

typedef struct {
  size_t tx, ty;
  PixelArenaData* data;
} PixelArena;


int pix_ArenaInit(PixelArena* pxa, size_t tx, size_t ty);
void pix_ArenaFree(PixelArena* pxa);
int pix_ArenaFill(PixelArena* pxa, uint8_t fill);
int pix_ArenaDraw(PixelArena* pxa);
int pix_ArenaSetPix(PixelArena* pxa, unsigned int x, unsigned int y, bool state);

#endif
