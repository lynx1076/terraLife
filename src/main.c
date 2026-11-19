#include "pixel.h"
#include "result.h"
#include "world.h"
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
    PIXEL_BORN,
    PIXEL_ALIVE,
    PIXEL_DIED,
    PIXEL_DEAD,
  };

  Vector4 colors[4] = {
    { 0.22f, 0.05f, 1.0f, 1.0f },   // Born
    { 0.22f, 0.8f, 1.0f, 1.0f },    // Alive
    { 0.27f, 0.27f, 0.3f, 1.0f },   // Died
    { 0.08f, 0.09f, 0.12f, 1.0f },  // Dead
  };
  
  SetShaderValueV(shader_arena, loc_states, states, SHADER_UNIFORM_UINT, 4);
  SetShaderValueV(shader_arena, loc_colors, colors, SHADER_UNIFORM_VEC4, 4);
 
  camera = (Camera2D){ 0 };
  camera.target = (Vector2){0, 0};
  camera.offset = (Vector2){GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  World world;
  if (wod_Init(&world)) return _res;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);

    BeginShaderMode(shader_arena);
    if (wod_Draw(&world)) return _res;
    EndShaderMode();

    EndDrawing();
  }

  UnloadShader(shader_arena);

  return 0;
}

