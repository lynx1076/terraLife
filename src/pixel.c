#include "pixel.h"
#include "result.h"
#include "debug.h"
#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

int pix_PixelArenaInit(PixelArena* pxa, PixelCoords pos) {
  ASSERT(pxa);

  PixelArenaData* data = malloc(sizeof(PixelArenaData));
  if (data == NULL) RES_RETURN(r_EMEM, -1);

  pxa->data = data;
  pxa->pos = pix_GlobalToArenaCoords(pos);

  if (pix_PixelArenaFill(pxa, 0)) return -1;

  Image img = {
    .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE,
    .height = PIXEL_ARENA_SIZE,
    .width = PIXEL_ARENA_SIZE,
    .mipmaps = 1,
    .data = pxa->data->pixels,
  };

  pxa->data->tex = LoadTextureFromImage(img);
  SetTextureFilter(pxa->data->tex, TEXTURE_FILTER_POINT);

  RES_RETURN(r_ENONE, 0);
}

void pix_PixelArenaFree(PixelArena* pxa) {
  ASSERT(pxa);

  UnloadTexture(pxa->data->tex);
  free(pxa->data);
}

int pix_PixelArenaFill(PixelArena* pxa, PixelState fill) {
  ASSERT(pxa);

  for (size_t y = 0; y < PIXEL_ARENA_SIZE; y++) {
    for (size_t x = 0; x < PIXEL_ARENA_SIZE; x++) {
      if (pix_PixelArenaSetPix(pxa, x, y, fill)) return -1;
    }
  }

  RES_RETURN(r_ENONE, 0);
}

int pix_PixelArenaDraw(PixelArena* pxa) {
  ASSERT(pxa);

  UpdateTexture(pxa->data->tex, pxa->data->pixels);
  Vector2 pos = {pxa->pos.x * PIXEL_SCALE, pxa->pos.y * PIXEL_SCALE};
  DrawTextureEx(pxa->data->tex, pos, 0, PIXEL_SCALE, WHITE);

  RES_RETURN(r_ENONE, 0);
}

int pix_PixelArenaSetPix(PixelArena* pxa, unsigned int x, unsigned int y, PixelState state) {
  ASSERT(pxa);

  pxa->data->pixels[y][x] = state;

  RES_RETURN(r_ENONE, 0);
}

PixelCoords pix_GlobalToArenaCoords(PixelCoords pos) {
  return (PixelCoords){
    .x = pos.x - pos.x % PIXEL_ARENA_SIZE,
    .y = pos.y - pos.y % PIXEL_ARENA_SIZE,
  };
}

