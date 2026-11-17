#include "pixel.h"
#include "result.h"
#include <raylib.h>
#include <rlgl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

char* window_title = "terraLife";
int window_width = 600;
int window_height = 500;

Camera2D camera;

int main(int argc, const char** argv) {
  (void)argc;
  (void)argv;

  SetTraceLogLevel(LOG_WARNING);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(window_width, window_height, window_title);
  SetExitKey(KEY_NULL);

  Shader shader_arena = LoadShader(0, "./shader/arena.fs");
  if (shader_arena.id == rlGetShaderIdDefault() || !IsShaderValid(shader_arena)) {
    printf("Failed to load shader!\n");
    RES_RETURN(r_ESHADER, -1);
  }

  int loc_states = GetShaderLocation(shader_arena, "states");
  int loc_colors = GetShaderLocation(shader_arena, "colors");

  uint32_t states[4] = {
    0b10000001,
    0b10000000,
    0b01000000,
    0b00000001,
  };

  Vector4 colors[4] = {
    { 0.22f, 0.8f, 1.0f, 1.0f }, // State 3: Mature
    { 0.22f, 0.05f, 1.0f, 1.0f }, // State 2: Born
    { 0.90f, 0.25f, 0.35f, 1.0f }, // State 1: Died
    { 0.08f, 0.09f, 0.12f, 1.0f }, // State 0: Dead
  };
  
  SetShaderValueV(shader_arena, loc_states, states, SHADER_UNIFORM_UINT, 4);
  SetShaderValueV(shader_arena, loc_colors, colors, SHADER_UNIFORM_VEC4, 4);
 
  camera = (Camera2D){ 0 };
  camera.target = (Vector2){0, 0};
  camera.offset = (Vector2){GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  PixelArena pxa;
  if (pix_ArenaInit(&pxa, 0, 0)) return _res;
  if (pix_ArenaSetPix(&pxa, 0, 0, true)) return _res;
  if (pix_ArenaSetPix(&pxa, 20, 0, true)) return _res;
  if (pix_ArenaSetPix(&pxa, 20, 20, true)) return _res;
  if (pix_ArenaSetPix(&pxa, 20, 20, true)) return _res;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);

    BeginShaderMode(shader_arena);
    if (pix_ArenaDraw(&pxa)) return _res;
    EndShaderMode();

    EndDrawing();
  }

  UnloadShader(shader_arena);

  return 0;
}

