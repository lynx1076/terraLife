#ifndef PIXEL_H
#define PIXEL_H

#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define PIXEL_ARENA_SIZE_POW2     2
#define PIXEL_ARENA_SIZE          (1 << (PIXEL_ARENA_SIZE_POW2))
#define PIXEL_SCALE               20

typedef enum {
  pix_Dead=-2,
  pix_Died=-1,
  pix_Alive=2,
  pix_Born=1,
} PixelState;

typedef struct {
  ssize_t x;
  ssize_t y;
} PixelCoords;

typedef struct {
  int8_t pixels_a[PIXEL_ARENA_SIZE][PIXEL_ARENA_SIZE];
  int8_t pixels_b[PIXEL_ARENA_SIZE][PIXEL_ARENA_SIZE];
  Texture2D tex;
  bool write_buff_a;
} ArenaData;

typedef struct {
  PixelCoords pos;
  ArenaData* data;
} Arena;


int pix_ArenaInit(Arena* pxa, PixelCoords pos);
void pix_ArenaFree(Arena* pxa);
int pix_ArenaFill(Arena* pxa, PixelState fill);
int pix_ArenaDraw(Arena* pxa);
PixelState pix_ArenaGetPix(Arena* pxa, PixelCoords pos);
void pix_ArenaSetPix(Arena* pxa, PixelCoords pos, PixelState state);
void pix_ArenaSwitchBuff(Arena* pxa);
PixelCoords pix_GlobalToArenaCoords(PixelCoords pos);

#endif
