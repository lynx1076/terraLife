#include "pixel.h"
#include "result.h"
#include "debug.h"
#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int pix_ArenaInit(Arena* pxa, PixelCoords pos) {
  ASSERT(pxa);

  ArenaData* data = malloc(sizeof(ArenaData));
  if (data == NULL) RES_RETURN(r_EMEM, -1);

  pxa->data = data;
  pxa->data->write_buff_a = false;
  pxa->pos = pix_GlobalToArenaCoords(pos);

  if (pix_ArenaFill(pxa, pix_Dead)) return -1;
  pix_ArenaSwitchBuff(pxa);
  if (pix_ArenaFill(pxa, pix_Dead)) return -1;

  Image img = {
    .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE,
    .height = PIXEL_ARENA_SIZE,
    .width = PIXEL_ARENA_SIZE,
    .mipmaps = 1,
    .data = pxa->data->pixels_a,
  };

  pxa->data->tex = LoadTextureFromImage(img);
  SetTextureFilter(pxa->data->tex, TEXTURE_FILTER_POINT);
  
  RES_RETURN(r_ENONE, 0);
}

void pix_ArenaFree(Arena* pxa) {
  ASSERT(pxa);

  UnloadTexture(pxa->data->tex);
  free(pxa->data);
}

int pix_ArenaFill(Arena* pxa, PixelState fill) {
  ASSERT(pxa);

  for (size_t y = 0; y < PIXEL_ARENA_SIZE; y++) {
    for (size_t x = 0; x < PIXEL_ARENA_SIZE; x++) {
      pix_ArenaSetPix(pxa, (PixelCoords){x, y}, fill);
    }
  }

  RES_RETURN(r_ENONE, 0);
}

int pix_ArenaDraw(Arena* pxa) {
  ASSERT(pxa);

  if (!pxa->data->write_buff_a) UpdateTexture(pxa->data->tex, pxa->data->pixels_a);
  else UpdateTexture(pxa->data->tex, pxa->data->pixels_b);

  Vector2 pos = {pxa->pos.x * PIXEL_SCALE, pxa->pos.y * PIXEL_SCALE};
  DrawTextureEx(pxa->data->tex, pos, 0, PIXEL_SCALE, WHITE);

  RES_RETURN(r_ENONE, 0);
}

void pix_ArenaSetPix(Arena* pxa, PixelCoords pos, PixelState state) {
  ASSERT(pxa);

  pos.x = pos.x % PIXEL_ARENA_SIZE;
  pos.y = pos.y % PIXEL_ARENA_SIZE;

  if (pxa->data->write_buff_a) pxa->data->pixels_a[pos.y][pos.x] = state;
  else pxa->data->pixels_b[pos.y][pos.x] = state;
}

PixelState pix_ArenaGetPix(Arena* pxa, PixelCoords pos) {
  ASSERT(pxa);

  pos.x = pos.x % PIXEL_ARENA_SIZE;
  pos.y = pos.y % PIXEL_ARENA_SIZE;

  if (!pxa->data->write_buff_a) return pxa->data->pixels_a[pos.y][pos.x];
  else return pxa->data->pixels_b[pos.y][pos.x];
}

void pix_ArenaSwitchBuff(Arena* pxa) {
  ASSERT(pxa);

  pxa->data->write_buff_a = !pxa->data->write_buff_a;
}

PixelCoords pix_GlobalToArenaCoords(PixelCoords pos) {
  return (PixelCoords){
    (pos.x >> PIXEL_ARENA_SIZE_POW2) << PIXEL_ARENA_SIZE_POW2,
    (pos.y >> PIXEL_ARENA_SIZE_POW2) << PIXEL_ARENA_SIZE_POW2,
  };
}

