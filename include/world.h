#ifndef WORLD_H
#define WORLD_H

#include "vector.h"
#include "pixel.h"
#include <stddef.h>


typedef struct {
  Vec arenas;
  size_t iterations;
} World;


int wod_Init(World* world);
void wod_Free(World* world);
int wod_AddArena(World* world, PixelCoords pos);
PixelArena* wod_GetArena(World* world, PixelCoords pos);
int wod_Draw(World* world);

#endif
