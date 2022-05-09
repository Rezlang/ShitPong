#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "raylib.h"

void unload();

int intro() {
  Sound music = LoadSound("rsc/startup.mp3");
  SetSoundVolume(music, 0.5);
  PlaySound(music);

  BeginDrawing();
  ClearBackground(WHITE);
  EndDrawing();
  BeginDrawing();
  ClearBackground(WHITE);
  EndDrawing();

  for (int i = 0; i < 280; ++i) {
    BeginDrawing();
    ClearBackground(WHITE);
    int textWidth = MeasureText("PONG", 100);
    DrawText("PONG", GetScreenWidth() / 3 * 2 - textWidth / 2 - (i / 5) + 50,
             GetScreenHeight() / 2 + 10, 100, YELLOW);

    textWidth = MeasureText("SHITARI", 100);
    DrawText("SHITARI", GetScreenWidth() / 3 - textWidth / 2 + (i / 5) + 50,
             GetScreenHeight() / 2 - 100, 100, BLACK);
    EndDrawing();
  }

  for (int i = 0; i < 10; ++i) {
    BeginDrawing();
    DrawRectangle(GetScreenWidth() / 10 + 50, GetScreenHeight() / 2,
                  GetScreenWidth() / 3 * 2 + 20, 5, BLACK);
    int textWidth = MeasureText("Press Space", 30);
    DrawText("Press Space", GetScreenWidth() / 2 - textWidth / 2,
             GetScreenHeight() / 4 * 3, 30, BLACK);
    EndDrawing();
  }

  while (!IsKeyPressed(KEY_SPACE)) {
    if (WindowShouldClose()) {
      UnloadSound(music);
      unload();
      return 1;
    }
    BeginDrawing();
    EndDrawing();
  }
  UnloadSound(music);
  return 0;
}

int controls(std::vector<int> keys) {
  Sound boop = LoadSound("rsc/boop.wav");
  SetSoundVolume(boop, 0.5);

  BeginDrawing();
  ClearBackground(BLACK);
  EndDrawing();
  BeginDrawing();
  ClearBackground(BLACK);
  EndDrawing();

  Texture2D upArrow = LoadTexture("rsc/upArrow.png");
  Texture2D downArrow = LoadTexture("rsc/downArrow.png");

  while (!IsKeyPressed(KEY_SPACE)) {
    if (WindowShouldClose()) {
      unload();
      return 1;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    int textWidth = MeasureText((char*)(&keys[0]), 60);
    DrawText((char*)(&keys[0]), GetScreenWidth() / 3 - textWidth / 3,
             GetScreenHeight() / 3 - 30, 60, LIME);
    textWidth = MeasureText((char*)(&keys[1]), 60);
    DrawText((char*)(&keys[1]), GetScreenWidth() / 3 - textWidth / 3,
             GetScreenHeight() / 3 * 2 - 30, 60, RED);
    textWidth = MeasureText((char*)(&keys[2]), 60);
    DrawText((char*)(&keys[2]), GetScreenWidth() / 3 * 2 - textWidth / 3,
             GetScreenHeight() / 3 - 30, 60, LIME);
    textWidth = MeasureText((char*)(&keys[3]), 60);
    DrawText((char*)(&keys[3]), GetScreenWidth() / 3 * 2 - textWidth / 3,
             GetScreenHeight() / 3 * 2 - 30, 60, RED);

    DrawTexture(upArrow, GetScreenWidth() / 2 - upArrow.width / 2,
                GetScreenHeight() / 3 - upArrow.width / 2, WHITE);

    DrawTexture(downArrow, GetScreenWidth() / 2 - downArrow.width / 2,
                GetScreenHeight() / 3 * 2 - downArrow.width / 2, WHITE);

    textWidth = MeasureText("Press Space", 30);
    DrawText("Press Space", GetScreenWidth() / 2 - textWidth / 2,
             GetScreenHeight() / 2 - 15, 30, YELLOW);

    EndDrawing();
  }

  PlaySound(boop);
  for (int i = 0; i < 20; ++i) {
    BeginDrawing();
    EndDrawing();
  }

  UnloadTexture(upArrow);
  UnloadTexture(downArrow);
  UnloadSound(boop);

  return 0;
}