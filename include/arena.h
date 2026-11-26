#ifndef ARENA_H
#define ARENA_H

#include "pixel.h"
#include <raylib.h>
#include <stddef.h>
#include <stdint.h>

#define PIXEL_ARENA_SIZE          32
#define PIXEL_SCALE               20

typedef struct {
  int8_t pixels_a[PIXEL_ARENA_SIZE][PIXEL_ARENA_SIZE];
  int8_t pixels_b[PIXEL_ARENA_SIZE][PIXEL_ARENA_SIZE];
  Texture2D tex;
  bool write_buff_a;
  bool empty;
} ArenaData;

typedef struct {
  PixelCoords pos;
  ArenaData* data;
} Arena;


int pix_ArenaInit(Arena* arena, PixelCoords pos);
void pix_ArenaFree(Arena* arena);
int pix_ArenaFill(Arena* arena, PixelState fill);
int pix_ArenaDraw(Arena* arena);
int pix_ArenaDrawDebug(Arena* arena);
PixelState pix_ArenaGetPix(Arena* arena, PixelCoords pos);
void pix_ArenaSetPix(Arena* arena, PixelCoords pos, PixelState state);
void pix_ArenaSwitchBuff(Arena* arena);
PixelCoords pix_GlobalToArenaCoords(PixelCoords pos);
size_t pix_ArenaCoordsToSqSpiral(PixelCoords pos);

#endif
