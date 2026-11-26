#include "color_scheme.h"
#include "gui.h"
#include "result.h"
#include "world.h"
#include <raylib.h>
#include <rlgl.h>
#include <stddef.h>
#include <stdint.h>


char* window_title = "terraLife";
int window_width = 600;
int window_height = 500;

Camera2D camera;

int main(int argc, const char** argv) {
  (void)argc;
  (void)argv;

#ifdef GRAPHICS_API_OPENGL_33
  printf("Defined 33\n");
#endif

#ifdef GRAPHICS_API_OPENGL_43
  printf("Defined 43\n");
#endif

  SetTraceLogLevel(LOG_WARNING);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(window_width, window_height, window_title);
  SetExitKey(KEY_NULL);

  World world;
  if (wod_Init(&world)) return _res;

  if (wod_SetPixel(&world, (PixelCoords){1, 0}, pix_Born)) return _res;
  if (wod_SetPixel(&world, (PixelCoords){3, 0}, pix_Born)) return _res;

  if (wod_SetPixel(&world, (PixelCoords){0, 1}, pix_Born)) return _res;

  if (wod_SetPixel(&world, (PixelCoords){1, 2}, pix_Born)) return _res;
  if (wod_SetPixel(&world, (PixelCoords){4, 2}, pix_Born)) return _res;

  if (wod_SetPixel(&world, (PixelCoords){3, 3}, pix_Born)) return _res;
  if (wod_SetPixel(&world, (PixelCoords){4, 3}, pix_Born)) return _res;
  if (wod_SetPixel(&world, (PixelCoords){5, 3}, pix_Born)) return _res;

  if (wod_Flip(&world)) return -1;

  GUI gui;
  if (gui_Init(&gui, &world)) return _res;

  while (!WindowShouldClose()) {
    if (gui_Update(&gui)) return -1;

    BeginDrawing();
    ClearBackground(color_bg);

    if (gui_Draw(&gui, &world)) return -1;

    EndDrawing();
  }

  return 0;
}

