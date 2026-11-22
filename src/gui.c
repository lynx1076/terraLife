#include "gui.h"
#include "debug.h"
#include "result.h"
#include "pixel.h"
#include "world.h"
#include <raylib.h>
#include <rlgl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


int gui_Init(GUI* gui) {
  ASSERT(gui);
  
  gui->cam = (Camera2D){ 0 };
  gui->cam.target = (Vector2){0, 0};
  gui->cam.offset = (Vector2){GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f};
  gui->cam.rotation = 0.0f;
  gui->cam.zoom = 1.0f;

  gui->cell_shader = LoadShader(0, "./shader/arena.fs");
  if (gui->cell_shader.id == rlGetShaderIdDefault() || !IsShaderValid(gui->cell_shader)) {
    printf("Failed to load shader!\n");
    RES_RETURN(r_ESHADER, -1);
  }

  int loc_states = GetShaderLocation(gui->cell_shader, "states");
  int loc_colors = GetShaderLocation(gui->cell_shader, "colors");

  int32_t states[3] = {
    pix_Born,
    pix_Alive,
    pix_Died,
  };

  Vector4 colors[3] = {
    { 0.22f, 0.8f, 1.0f, 1.0f },    // Born
    { 0.22f, 0.05f, 1.0f, 1.0f },   // Alive
    { 0.27f, 0.27f, 0.3f, 1.0f },   // Died
  };
  
  SetShaderValueV(gui->cell_shader, loc_states, states, SHADER_UNIFORM_INT, 3);
  SetShaderValueV(gui->cell_shader, loc_colors, colors, SHADER_UNIFORM_VEC4, 3);

  RES_RETURN(r_ENONE, 0);
}

void gui_Free(GUI *gui) {
  ASSERT(gui);

  UnloadShader(gui->cell_shader);
}

int gui_Update(GUI* gui) {
  ASSERT(gui);

  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    Vector2 delta = GetMouseDelta();
    gui->cam.target.x -= delta.x / gui->cam.zoom;
    gui->cam.target.y -= delta.y / gui->cam.zoom;
  }

  float scroll = GetMouseWheelMove();
  if (scroll) {
    gui->cam.zoom += scroll * 0.1;
    if (gui->cam.zoom < GUI_MIN_ZOOM) gui->cam.zoom = GUI_MIN_ZOOM;
    else if (gui->cam.zoom > GUI_MAX_ZOOM) gui->cam.zoom = GUI_MAX_ZOOM;
  }

  if (IsKeyPressed(KEY_Q)) {
    gui->cam.target = (Vector2){0, 0};
    gui->cam.zoom = 1.0f;
  }

  RES_RETURN(r_ENONE, 0);
}

int gui_Draw(GUI* gui, World* world) {
  ASSERT(gui);
 
  BeginMode2D(gui->cam);

  BeginShaderMode(gui->cell_shader);
  if (wod_Draw(world)) return _res;
  EndShaderMode();

  EndMode2D();

  RES_RETURN(r_ENONE, 0);
}

