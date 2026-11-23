#ifndef GUI_H
#define GUI_H

#include "pixel.h"
#include "world.h"
#include <raylib.h>
#include <stddef.h>


#define GUI_MIN_ZOOM            0.08f
#define GUI_MAX_ZOOM            2

#define GUI_MIN_MS_PER_G        10
#define GUI_MAX_MS_PER_G        1000

typedef struct {
  Camera2D cam;
  Shader cell_shader;
  World* world;

  PixelCoords last_target;
  bool had_target;
  bool auto_step;
  unsigned int ms_per_gen;
  size_t last_gen_ms;
} GUI;

int gui_Init(GUI* gui, World* world);
void gui_Free(GUI* gui);
int gui_Update(GUI* gui);
int gui_Draw(GUI* gui, World* world);

#endif
