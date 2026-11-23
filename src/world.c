#include "world.h"
#include "debug.h"
#include "pixel.h"
#include "result.h"
#include "vector.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


int wod_Init(World* world) {
  ASSERT(world);

  if (vec_init(&world->arenas, sizeof(Arena))) return -1;

  Arena pix;
  if (pix_ArenaInit(&pix, (PixelCoords){0, 0})) return -1;
  if (vec_push(&world->arenas, &pix)) return -1;
 
  for (int i = 0; i < 9; i++) {
    world->n_surv[i] = false;
    world->n_birth[i] = false;
  }

  world->n_surv[2] = true;
  world->n_surv[3] = true;
  world->n_birth[3] = true;

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

void wod_Print(World* world) {
  if (!world) {
    printf("World is NULL\n");
  }

  printf("World:\n");
  printf("Arenas: %zu\n", world->arenas.count);
  for (size_t i = 0; i < world->arenas.count; i++) {
    Arena* arena = vec_get(&world->arenas, i);
    printf("[%zu]: %zi, %zi\n", i, arena->pos.x, arena->pos.y);
  }
}

int wod_AddArena(World* world, PixelCoords pos) {
  ASSERT(world);

  size_t low = 0;
  size_t high = world->arenas.count;
  size_t mid ;

  size_t goal = pix_ArenaCoordsToSqSpiral(pos);

  while (low <= high) {
    mid = low + (high - low) / 2;

    Arena* arena = vec_get(&world->arenas, mid);
    if (!arena) break;
    size_t curr = pix_ArenaCoordsToSqSpiral(arena->pos);
    if (curr == goal) {
      RES_RETURN(r_ENONE, 0);
    }
    if (curr < goal) low = mid + 1;
    else high = mid - 1;
  }

  Arena arena;
  if (pix_ArenaInit(&arena, pos)) return -1;
  if (vec_push_to(&world->arenas, low, &arena)) return -1;

  RES_RETURN(r_ENONE, 0);
}

Arena* wod_GetArena(World* world, PixelCoords pos) {
  ASSERT(world);

  size_t low = 0;
  size_t high = world->arenas.count;
  size_t mid ;

  size_t goal = pix_ArenaCoordsToSqSpiral(pos);

  while (low <= high) {
    mid = low + (high - low) / 2;

    Arena* arena = vec_get(&world->arenas, mid);
    if (!arena) RES_RETURN(r_ENONE, NULL);
    size_t curr = pix_ArenaCoordsToSqSpiral(arena->pos);
    if (curr == goal) {
      RES_RETURN(r_ENONE, arena);
    }
    if (curr < goal) low = mid + 1;
    else high = mid - 1;
  }

  RES_RETURN(r_ENONE, NULL);
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

int wod_Flip(World* world) {
  ASSERT(world);

  for (size_t i = 0; i < world->arenas.count; i++) {
    Arena* arena = vec_get(&world->arenas, i);
    pix_ArenaSwitchBuff(arena);
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
      if (pix > 0) count++;
    }
  }

  ASSERT(count <= 8);
  *n = count;

  RES_RETURN(r_ENONE, 0);
}

int wod_AllocateArenas(World* world) {
  ASSERT(world);

  Vec new_arenas;
  if (vec_init(&new_arenas, sizeof(PixelCoords))) return -1;

  for (size_t i = 0; i < world->arenas.count; i++) {
    Arena* arena = vec_get(&world->arenas, i);

    for (size_t y = 0; y < PIXEL_ARENA_SIZE; y++) {
      for (size_t x = 0; x < PIXEL_ARENA_SIZE; x++) {
        PixelCoords pos = {
          x, 
          y,
        };

        if ((x != 0 && x != PIXEL_ARENA_SIZE - 1) && y != 0 && y != PIXEL_ARENA_SIZE - 1) {
          continue;
        }

        PixelState pix = pix_ArenaGetPix(arena, pos);
        if (pix <= 0) continue;

        PixelCoords new_arena_pos = (PixelCoords){
          pos.x == 0 ? arena->pos.x - 1 : pos.x == PIXEL_ARENA_SIZE - 1 ? arena->pos.x + PIXEL_ARENA_SIZE : arena->pos.x, 
          pos.y == 0 ? arena->pos.y - 1 : pos.y == PIXEL_ARENA_SIZE - 1 ? arena->pos.y + PIXEL_ARENA_SIZE : arena->pos.y
        };

        if (vec_push(&new_arenas, &new_arena_pos)) return -1;
      }
    }
  }

  for (size_t i = 0; i < new_arenas.count; i++) {
    PixelCoords* pos = vec_get(&new_arenas, i);
    if (wod_AddArena(world, *pos)) return -1;
  }

  vec_free(&new_arenas);

  RES_RETURN(r_ENONE, 0);
}

int wod_StepWorld(World* world) {
  ASSERT(world);

  if (wod_AllocateArenas(world)) return -1;

  for (size_t i = 0; i < world->arenas.count; i++) {
    Arena* arena = vec_get(&world->arenas, i);
    if (wod_StepArena(world, arena)) return -1;
  }

  if (wod_Flip(world)) return -1;

  RES_RETURN(r_ENONE, 0);
}

int wod_StepArena(World* world, Arena* arena) {
  ASSERT(world);
  ASSERT(arena);

  for (size_t y = 0; y < PIXEL_ARENA_SIZE; y++) {
    for (size_t x = 0; x < PIXEL_ARENA_SIZE; x++) {
      PixelCoords pos = {
        arena->pos.x + x, 
        arena->pos.y + y, 
      };

      PixelCoords arena_pos = {
        x, 
        y, 
      };

      PixelState pix = pix_ArenaGetPix(arena, arena_pos);

      int n;
      if (wod_CountNeighbours(world, pos, &n)) return -1;

      PixelState new_state = pix;

      if (world->n_birth[n] && pix <= 0) new_state = pix_Born;
      else if (!world->n_surv[n] && pix > 0) new_state = pix_Died;

      if (new_state < 0 && pix < 0) new_state = pix_Dead;
      else if (new_state > 0 && pix > 0) new_state = pix_Alive;

      pix_ArenaSetPix(arena, arena_pos, new_state);
    }
  }

  RES_RETURN(r_ENONE, 0);
}

