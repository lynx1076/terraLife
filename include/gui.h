#ifndef GUI_H
#define GUI_H

#include "world.h"
#include <raylib.h>


#define GUI_MIN_ZOOM            0.01f
#define GUI_MAX_ZOOM            2

typedef struct {
  Vector2 mouseClickStart;
  Camera2D cam;
  Shader cell_shader;
} GUI;

int gui_Init(GUI* gui);
void gui_Free(GUI* gui);
int gui_Update(GUI* gui);
int gui_Draw(GUI* gui, World* world);

#endif
