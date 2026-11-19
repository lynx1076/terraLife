#include "world.h"
#include "debug.h"
#include "pixel.h"
#include "result.h"
#include "vector.h"
#include <stddef.h>


int wod_Init(World* world) {
  ASSERT(world);

  if (vec_init(&world->arenas, sizeof(PixelArena))) return -1;

  PixelArena pix;
  if (pix_PixelArenaInit(&pix, (PixelCoords){0, 0})) return -1;
  if (vec_push(&world->arenas, &pix)) return -1;

  world->iterations = 0;

  RES_RETURN(r_ENONE, 0);
}

void wod_Free(World* world) {
  ASSERT(world);

  for (size_t i = 0; i < world->arenas.count; i++) {
    PixelArena* arena = vec_get(&world->arenas, i);

    pix_PixelArenaFree(arena);
  }

  vec_free(&world->arenas);
}

int wod_AddArena(World* world, PixelCoords pos) {
  ASSERT(world);

  PixelCoords arena_pos = pix_GlobalToArenaCoords(pos);

  PixelArena pxa;
  if (pix_PixelArenaInit(&pxa, arena_pos)) return -1;

  if (vec_push(&world->arenas, &pxa)) return -1;

  RES_RETURN(r_ENONE, 0);
}

PixelArena* wod_GetArena(World* world, PixelCoords pos) {
  ASSERT(world);

  PixelCoords arena_pos = pix_GlobalToArenaCoords(pos);
  
  size_t a = 0;
  size_t b = world->arenas.count;

  size_t i = (a + b) / 2;

  PixelArena* arena = vec_get(&world->arenas, i);
  while (arena) {
    if (arena->pos.x == arena_pos.x && arena->pos.y == arena_pos.y) {
      return arena;
    }
    
    if (arena->pos.x > arena_pos.x || (arena->pos.y > arena_pos.y && arena->pos.x == arena_pos.x)) {
      b = i;
    } else {
      a = i;
    }
 
    if (a == b - 1) {
      if (i == b) return NULL;
      i = b;
    }
    else i = (a + b) / 2;

    arena = vec_get(&world->arenas, i);
  }

  return NULL;
}

int wod_Draw(World* world) {
  ASSERT(world);

  for (size_t i = 0; i < world->arenas.count; i++) {
    PixelArena* arena = vec_get(&world->arenas, i);
    if (pix_PixelArenaDraw(arena)) return -1;
  }

  RES_RETURN(r_ENONE, 0);
}

