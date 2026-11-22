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

  int32_t states[3] = {
    pix_Born,
    pix_Alive,
    pix_Died,
  };

  Vector4 colors[3] = {
    { 0.22f, 0.05f, 1.0f, 1.0f },   // Born
    { 0.22f, 0.8f, 1.0f, 1.0f },    // Alive
    { 0.27f, 0.27f, 0.3f, 1.0f },   // Died
  };
  
  SetShaderValueV(shader_arena, loc_states, states, SHADER_UNIFORM_INT, 3);
  SetShaderValueV(shader_arena, loc_colors, colors, SHADER_UNIFORM_VEC4, 3);
 
  camera = (Camera2D){ 0 };
  camera.target = (Vector2){0, 0};
  camera.offset = (Vector2){GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  World world;
  if (wod_Init(&world)) return _res;

  if (wod_SetPixel(&world, (PixelCoords){0, 2}, pix_Born)) return _res;
  if (wod_SetPixel(&world, (PixelCoords){1, 2}, pix_Born)) return _res;
  if (wod_SetPixel(&world, (PixelCoords){1, 0}, pix_Born)) return _res;
  if (wod_SetPixel(&world, (PixelCoords){2, 2}, pix_Born)) return _res;
  if (wod_SetPixel(&world, (PixelCoords){2, 1}, pix_Born)) return _res;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground((Color){20, 23, 31, 0xFF});

    if (IsKeyPressed(KEY_SPACE)) {
      wod_StepWorld(&world);
    }

    BeginShaderMode(shader_arena);
    if (wod_Draw(&world)) return _res;
    EndShaderMode();

    EndDrawing();
  }

  UnloadShader(shader_arena);

  return 0;
}

