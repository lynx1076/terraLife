#ifndef WORLD_H
#define WORLD_H

#include "vector.h"
#include "pixel.h"
#include <stddef.h>
#include <stdint.h>


typedef struct {
  Vec arenas;
  size_t iterations;

  bool n_surv[9];
  bool n_birth[9];
} World;


int wod_Init(World* world);
void wod_Free(World* world);
int wod_AddArena(World* world, PixelCoords pos);
Arena* wod_GetArena(World* world, PixelCoords pos);
PixelState wod_GetPixel(World* world, PixelCoords pos);
int wod_SetPixel(World* world, PixelCoords pos, PixelState state);
int wod_Draw(World* world);

int wod_CountNeighbours(World* world, PixelCoords pos, int* n);
int wod_StepWorld(World* world);
int wod_StepArena(World* world, Arena* arena);

#endif
