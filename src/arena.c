#include "arena.h"
#include "utils.h"
#include "result.h"
#include "debug.h"
#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color_scheme.h"


int pix_ArenaInit(Arena* arena, PixelCoords pos) {
  ASSERT(arena);

  ArenaData* data = malloc(sizeof(ArenaData));
  if (data == NULL) RES_RETURN(r_EMEM, -1);

  arena->data = data;
  arena->data->write_buff_a = false;
  arena->data->empty = true;
  arena->pos = pix_GlobalToArenaCoords(pos);

  if (pix_ArenaFill(arena, pix_Dead)) return -1;
  pix_ArenaSwitchBuff(arena);
  if (pix_ArenaFill(arena, pix_Dead)) return -1;

  Image img = {
    .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE,
    .height = PIXEL_ARENA_SIZE,
    .width = PIXEL_ARENA_SIZE,
    .mipmaps = 1,
    .data = arena->data->pixels_a,
  };

  arena->data->tex = LoadTextureFromImage(img);
  SetTextureFilter(arena->data->tex, TEXTURE_FILTER_POINT);
  
  RES_RETURN(r_ENONE, 0);
}

void pix_ArenaFree(Arena* arena) {
  ASSERT(arena);

  UnloadTexture(arena->data->tex);
  free(arena->data);
}

int pix_ArenaFill(Arena* arena, PixelState fill) {
  ASSERT(arena);

  for (size_t y = 0; y < PIXEL_ARENA_SIZE; y++) {
    for (size_t x = 0; x < PIXEL_ARENA_SIZE; x++) {
      pix_ArenaSetPix(arena, (PixelCoords){x, y}, fill);
    }
  }

  RES_RETURN(r_ENONE, 0);
}

int pix_ArenaDraw(Arena* arena) {
  ASSERT(arena);

  if (arena->data->empty) RES_RETURN(r_ENONE, 0);

  if (!arena->data->write_buff_a) UpdateTexture(arena->data->tex, arena->data->pixels_a);
  else UpdateTexture(arena->data->tex, arena->data->pixels_b);

  Vector2 pos = {arena->pos.x * PIXEL_SCALE, arena->pos.y * PIXEL_SCALE};
  DrawTextureEx(arena->data->tex, pos, 0, PIXEL_SCALE, WHITE);

  RES_RETURN(r_ENONE, 0);
}

int pix_ArenaDrawDebug(Arena* arena) {
  ASSERT(arena);

#ifndef DEBUG
  RES_RETURN(r_ENONE, 0);
#endif

  if (arena->data->empty) {
    DrawRectangle(arena->pos.x * PIXEL_SCALE, arena->pos.y * PIXEL_SCALE, PIXEL_ARENA_SIZE * PIXEL_SCALE, PIXEL_ARENA_SIZE * PIXEL_SCALE, color_highlight_yellow);
  } else {
    DrawRectangle(arena->pos.x * PIXEL_SCALE, arena->pos.y * PIXEL_SCALE, PIXEL_ARENA_SIZE * PIXEL_SCALE, PIXEL_ARENA_SIZE * PIXEL_SCALE, color_highlight_green);
  }

  RES_RETURN(r_ENONE, 0);
}

void pix_ArenaSetPix(Arena* arena, PixelCoords pos, PixelState state) {
  ASSERT(arena);

  pos.x = pos.x % PIXEL_ARENA_SIZE;
  pos.y = pos.y % PIXEL_ARENA_SIZE;

  if (pos.x < 0) pos.x = PIXEL_ARENA_SIZE + pos.x;
  if (pos.y < 0) pos.y = PIXEL_ARENA_SIZE + pos.y;

  if (arena->data->write_buff_a) arena->data->pixels_a[pos.y][pos.x] = state;
  else arena->data->pixels_b[pos.y][pos.x] = state;

  if (state != pix_Dead) arena->data->empty = false;
}

PixelState pix_ArenaGetPix(Arena* arena, PixelCoords pos) {
  ASSERT(arena);

  pos.x = pos.x % PIXEL_ARENA_SIZE;
  pos.y = pos.y % PIXEL_ARENA_SIZE;

  if (pos.x < 0) pos.x = PIXEL_ARENA_SIZE + pos.x;
  if (pos.y < 0) pos.y = PIXEL_ARENA_SIZE + pos.y;

  if (!arena->data->write_buff_a) return arena->data->pixels_a[pos.y][pos.x];
  else return arena->data->pixels_b[pos.y][pos.x];
}

void pix_ArenaSwitchBuff(Arena* arena) {
  ASSERT(arena);

  if (arena->data->empty) return;

  arena->data->write_buff_a = !arena->data->write_buff_a;
}

void pix_ArenaSetBuffEmpty(Arena* arena) {
  ASSERT(arena);

  if (arena->data->empty) return;

  pix_ArenaSwitchBuff(arena);
  arena->data->empty = true;
}

PixelCoords pix_GlobalToArenaCoords(PixelCoords pos) {
  return (PixelCoords){
    FloorDiv(pos.x, PIXEL_ARENA_SIZE) * PIXEL_ARENA_SIZE,
    FloorDiv(pos.y, PIXEL_ARENA_SIZE) * PIXEL_ARENA_SIZE,
  };
}

size_t pix_ArenaCoordsToSqSpiral(PixelCoords pos) {
  pos = (PixelCoords){
    FloorDiv(pos.x, PIXEL_ARENA_SIZE),
    FloorDiv(pos.y, PIXEL_ARENA_SIZE),
  };

  ssize_t x = pos.x;
  if (x >= 0) x = 2 * x;
  else x = -2 * x - 1;

  ssize_t y = pos.y;
  if (y >= 0) y = 2 * y;
  else y = -2 * y - 1;

  ssize_t res = (x + y) * (x + y + 1) / 2 + y;

  return (size_t)res;
}

