#include "world.h"
#include "debug.h"
#include "pixel.h"
#include "result.h"
#include "vector.h"
#include <stdbool.h>
#include <stddef.h>


int wod_Init(World* world) {
  ASSERT(world);

  if (vec_init(&world->arenas, sizeof(Arena))) return -1;

  Arena pix;
  if (pix_ArenaInit(&pix, (PixelCoords){0, 0})) return -1;
  if (vec_push(&world->arenas, &pix)) return -1;
 
  for (int i = 0; i < 8; i++) {
    world->n_surv[i] = false;
    world->n_birth[i] = false;
  }

  world->n_surv[1] = true;
  world->n_surv[2] = true;
  world->n_birth[2] = true;

  world->iterations = 0;

  RES_RETURN(r_ENONE, 0);
}

void wod_Free(World* world) {
  ASSERT(world);

  for (size_t i = 0; i < world->arenas.count; i++) {
    Arena* arena = vec_get(&world->arenas, i);

    pix_ArenaFree(arena);
  }

  vec_free(&world->arenas);
}

int wod_AddArena(World* world, PixelCoords pos) {
  ASSERT(world);

  PixelCoords arena_pos = pix_GlobalToArenaCoords(pos);
  
  size_t a = 0;
  size_t b = world->arenas.count;

  size_t i = (a + b) / 2;

  Arena* arena = vec_get(&world->arenas, i);
  while (arena) {
    if (arena->pos.x == arena_pos.x && arena->pos.y == arena_pos.y) {
      RES_RETURN(r_ENONE, 0);
    }
    
    if (arena->pos.x > arena_pos.x || (arena->pos.y > arena_pos.y && arena->pos.x == arena_pos.x)) {
      b = i;
    } else {
      a = i;
    }
 
    if (a == b - 1) {
      if (i == b) break;
      i = b;
    }
    else i = (a + b) / 2;

    arena = vec_get(&world->arenas, i);
  }

  Arena pxa;
  if (pix_ArenaInit(&pxa, pos)) return -1;

  if (vec_push_to(&world->arenas, i, &pxa)) return -1;

  RES_RETURN(r_ENONE, 0);
}

Arena* wod_GetArena(World* world, PixelCoords pos) {
  ASSERT(world);

  PixelCoords arena_pos = pix_GlobalToArenaCoords(pos);
  
  size_t a = 0;
  size_t b = world->arenas.count;

  size_t i = (a + b) / 2;

  Arena* arena = vec_get(&world->arenas, i);
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

PixelState wod_GetPixel(World* world, PixelCoords pos) {
  ASSERT(world);

  Arena* pxa = wod_GetArena(world, pos);
  if (!pxa) return pix_Dead;

  return pix_ArenaGetPix(pxa, pos);
}

int wod_SetPixel(World* world, PixelCoords pos, PixelState state) {
  ASSERT(world);

  Arena* pxa = wod_GetArena(world, pos);
  if (!pxa) {
    if (wod_AddArena(world, pos)) return -1;
    pxa = wod_GetArena(world, pos);
  }
  pix_ArenaSetPix(pxa, pos, state);

  RES_RETURN(r_ENONE, 0);
}

int wod_Draw(World* world) {
  ASSERT(world);

  for (size_t i = 0; i < world->arenas.count; i++) {
    Arena* arena = vec_get(&world->arenas, i);
    if (pix_ArenaDraw(arena)) return -1;
  }

  RES_RETURN(r_ENONE, 0);
}

int wod_CountNeighbours(World* world, PixelCoords pos, int* n) {
  ASSERT(world);

  int count = 0;

  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      PixelCoords check_pos = {
        .x = pos.x + x - 1,
        .y = pos.y + y - 1,
      };
      if (y == 1 && x == 1) continue;
      PixelState pix = wod_GetPixel(world, check_pos);
      if (pix <= 0) count++;
    }
  }

  ASSERT(count <= 8);
  *n = count;

  RES_RETURN(r_ENONE, 0);
}

int wod_StepWorld(World* world) {
  ASSERT(world);

  for (size_t i = 0; i < world->arenas.count; i++) {
    Arena* arena = vec_get(&world->arenas, i);
    if (wod_StepArena(world, arena)) return -1;
  }

  for (size_t i = 0; i < world->arenas.count; i++) {
    Arena* pxa = vec_get(&world->arenas, i);
    pix_ArenaSwitchBuff(pxa);
  }

  RES_RETURN(r_ENONE, 0);
}

int wod_StepArena(World* world, Arena* arena) {
  ASSERT(world);
  ASSERT(arena);

  for (size_t y = 0; y < PIXEL_ARENA_SIZE; y++) {
    for (size_t x = 0; x < PIXEL_ARENA_SIZE; x++) {
      PixelCoords pos = {
        x, 
        y,
      };

      PixelState pix = pix_ArenaGetPix(arena, pos);

      int n;
      if (wod_CountNeighbours(world, pos, &n)) return -1;
      if (n || pix > 0) printf("Pixel (%zu, %zu) has %i neighbours\n", pos.x, pos.y, n);

      PixelState new_state = pix;

      if (world->n_birth[n]) new_state = pix_Born;
      else if (!world->n_surv[n]) new_state = pix_Died;

      if (new_state < 0 && pix < 0) new_state = pix_Dead;
      if (new_state > 0 && pix > 0) new_state = pix_Alive;

      if (wod_SetPixel(world, pos, new_state)) return -1;
    }
  }

  RES_RETURN(r_ENONE, 0);
}

