#include "gui.h"
#include "color_scheme.h"
#include "debug.h"
#include "result.h"
#include "pixel.h"
#include "utils.h"
#include "world.h"
#include <raylib.h>
#include <rlgl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <raygui.h>

int gui_Init(GUI* gui, World* world) {
  ASSERT(gui);
  ASSERT(world);
  
  gui->cam = (Camera2D){ 0 };
  gui->cam.target = (Vector2){0, 0};
  gui->cam.offset = (Vector2){GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f};
  gui->cam.rotation = 0.0f;
  gui->cam.zoom = 1.0f;

  gui->auto_step = false;
  gui->ms_per_gen = GUI_MAX_MS_PER_G;
  gui->last_gen_ms = 0;
  gui->had_target = false;

  gui->cell_shader = LoadShader(0, "./shader/arena.fs");
  if (gui->cell_shader.id == rlGetShaderIdDefault() || !IsShaderValid(gui->cell_shader)) {
    printf("Failed to load shader!\n");
    RES_RETURN(r_ESHADER, -1);
  }

  gui->world = world;

  int loc_states = GetShaderLocation(gui->cell_shader, "states");
  int loc_colors = GetShaderLocation(gui->cell_shader, "colors");

  int32_t states[3] = {
    pix_Born,
    pix_Alive,
    (uint8_t)pix_Died,
  };

  Vector4 colors[3] = {
    ColorNormalize(color_born),
    ColorNormalize(color_alive),
    ColorNormalize(color_died),
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

  bool moveView = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) || (IsKeyDown(KEY_LEFT_SHIFT) && IsMouseButtonDown(MOUSE_BUTTON_LEFT));
  if (moveView) {
    Vector2 delta = GetMouseDelta();
    gui->cam.target.x -= delta.x / gui->cam.zoom;
    gui->cam.target.y -= delta.y / gui->cam.zoom;
  }

  Vector2 target = GetScreenToWorld2D(GetMousePosition(), gui->cam);
  PixelCoords edit = {
    FloorDiv((long)target.x, PIXEL_SCALE),
    FloorDiv((long)target.y, PIXEL_SCALE),
  };

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !moveView) {
    if (edit.x != gui->last_target.x || edit.y != gui->last_target.y || !gui->had_target) {
      PixelState pix = wod_GetPixel(gui->world, edit);
      if (pix > 0) pix = pix_Died;
      else pix = pix_Born;

      if (wod_Flip(gui->world)) return -1;
      if (wod_SetPixel(gui->world, edit, pix)) return -1;
      if (wod_Flip(gui->world)) return -1;

      gui->last_target = (PixelCoords){
        edit.x,
        edit.y,
      };
      gui->had_target = true;
    }
  } else {
    gui->had_target = false;
  }

  float scroll = GetMouseWheelMove();
  if (scroll) {
    Vector2 target = GetScreenToWorld2D(GetMousePosition(), gui->cam);
    gui->cam.zoom += scroll * 0.1;
    if (gui->cam.zoom < GUI_MIN_ZOOM) gui->cam.zoom = GUI_MIN_ZOOM;
    else if (gui->cam.zoom > GUI_MAX_ZOOM) gui->cam.zoom = GUI_MAX_ZOOM;
    gui->cam.target = target;
    gui->cam.offset = GetMousePosition();
  }

  if (IsKeyPressed(KEY_RIGHT)) {
    gui->ms_per_gen *= 2;
  } else if (IsKeyPressed(KEY_LEFT)) {
    gui->ms_per_gen /= 2;
  }

  if (gui->ms_per_gen < GUI_MIN_MS_PER_G) gui->ms_per_gen = GUI_MIN_MS_PER_G;
  else if (gui->ms_per_gen > GUI_MAX_MS_PER_G) gui->ms_per_gen = GUI_MAX_MS_PER_G;

  GuiLabel((Rectangle){ 24, 68, 250, 24 },
      TextFormat("Auto step %s %ums", gui->auto_step ? "enabled" : "disabled", gui->ms_per_gen));

  if (IsKeyPressed(KEY_SPACE)) {
    gui->auto_step = !gui->auto_step;
  }

  if (gui->auto_step && !gui->had_target) {
    size_t millis = GetTime() * 1000;
    if (gui->last_gen_ms + gui->ms_per_gen * 2 < millis) {
      GuiLabel((Rectangle){ 24, 88, 250, 24 },
      TextFormat("Lagging behind! %ums", millis - gui->last_gen_ms));
    }
    if (gui->last_gen_ms + gui->ms_per_gen < millis) {
      gui->last_gen_ms = millis;
      if (wod_StepWorld(gui->world)) return -1;
    }
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

  Vector2 target = GetScreenToWorld2D(GetMousePosition(), gui->cam);
  PixelCoords edit = {
    FloorDiv((long)target.x, PIXEL_SCALE),
    FloorDiv((long)target.y, PIXEL_SCALE),
  };
  PixelCoords arena = pix_GlobalToArenaCoords(edit);

  DrawRectangle(edit.x * PIXEL_SCALE, edit.y * PIXEL_SCALE, PIXEL_SCALE, PIXEL_SCALE, color_highlight);
  DrawRectangle(arena.x * PIXEL_SCALE, arena.y * PIXEL_SCALE, PIXEL_ARENA_SIZE * PIXEL_SCALE, PIXEL_ARENA_SIZE * PIXEL_SCALE, color_highlight2);

  EndMode2D();

  /*
  static bool showMessage = false;
  if (GuiButton((Rectangle){ 24, 24, 140, 30 }, "Click Me")) {
    showMessage = !showMessage;
  }

  if (showMessage) {
    GuiLabel((Rectangle){ 24, 68, 250, 24 }, "Button was clicked!");
  }
  */

  RES_RETURN(r_ENONE, 0);
}

