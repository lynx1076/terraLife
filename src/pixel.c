#include "pixel.h"
#include "result.h"
#include "debug.h"
#include <raylib.h>
#include <stddef.h>
#include <stdint.h>

int pix_ArenaInit(PixelArena* pxa, size_t tx, size_t ty) {
  ASSERT(pxa);

  pxa->tx = tx;
  pxa->ty = ty;

  if (pix_ArenaFill(pxa, 0)) return -1;

  Image img = {
    .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE,
    .height = PIXEL_ARENA_SIZE,
    .width = PIXEL_ARENA_SIZE,
    .mipmaps = 1,
    .data = pxa->data->pixels,
  };

  pxa->tex = LoadTextureFromImage(img);
  SetTextureFilter(pxa->tex, TEXTURE_FILTER_POINT);

  RES_RETURN(r_ENONE, 0);
}

void pix_ArenaFree(PixelArena* pxa) {
  ASSERT(pxa);

  UnloadTexture(pxa->tex);
}

int pix_ArenaFill(PixelArena* pxa, uint8_t fill) {
  ASSERT(pxa);

  for (size_t y = 0; y < PIXEL_ARENA_SIZE; y++) {
    for (size_t x = 0; x < PIXEL_ARENA_SIZE; x++) {
      pxa->pixels[y][x] = fill;
    }
  }

  RES_RETURN(r_ENONE, 0);
}

int pix_ArenaDraw(PixelArena* pxa) {
  ASSERT(pxa);

  UpdateTexture(pxa->tex, pxa->pixels);
  Vector2 pos = {pxa->tx * PIXEL_SCALE, pxa->ty * PIXEL_SCALE};
  DrawTextureEx(pxa->tex, pos, 0, PIXEL_SCALE, WHITE);

  RES_RETURN(r_ENONE, 0);
}

int pix_ArenaSetPix(PixelArena* pxa, unsigned int x, unsigned int y, bool state) {
  ASSERT(pxa);

  uint8_t* pixel = &pxa->pixels[y][x];
  *pixel = (*pixel >> 1) | ((state ? 1 : 0) << 7);

  RES_RETURN(r_ENONE, 0);
}

int pix_ArenaUpdate(PixelArena* pxa) {
  ASSERT(pxa);



  RES_RETURN(r_ENONE, 0);
}

