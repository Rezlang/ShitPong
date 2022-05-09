#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "raylib.h"

class Ball {
 public:
  float x, y;
  float speedX, speedY;
  float radius;

  void Draw() { DrawCircle((int)x, (int)y, radius, WHITE); }
};

class Paddle {
 public:
  float x, y;
  float speed;
  float width, height;

  Rectangle GetRect() {
    return Rectangle{x - width / 2, y - height / 2, 10, 100};
  }

  void Draw() { DrawRectangleRec(GetRect(), WHITE); }
};

void unload() {
  CloseAudioDevice();
  CloseWindow();
}

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

int main() {
  InitWindow(800, 600, "ShitPong");
  InitAudioDevice();
  SetWindowState(FLAG_VSYNC_HINT);
  SetTargetFPS(60);
  srand(time(NULL));

  // Control Randomization
  int upLeft = rand() % 26 + 65;
  int downLeft = rand() % 26 + 65;
  int upRight = rand() % 26 + 65;
  int downRight = rand() % 26 + 65;
  std::set<int> keys;
  keys.insert(upLeft);
  keys.insert(downLeft);
  keys.insert(upRight);
  keys.insert(downRight);

  // reroll if not unique or if Y is selected
  while (keys.size() != 4 || keys.find(89) != keys.end()) {
    keys.clear();
    upLeft = rand() % 26 + 65;
    downLeft = rand() % 26 + 65;
    upRight = rand() % 26 + 65;
    downRight = rand() % 26 + 65;
    keys.insert(upLeft);
    keys.insert(downLeft);
    keys.insert(upRight);
    keys.insert(downRight);
  }

  Ball ball;
  ball.x = GetScreenWidth() / 2.0f;
  ball.y = GetScreenHeight() / 2.0f;
  ball.radius = 5;
  ball.speedX = 300;
  ball.speedY = 300;

  Paddle leftPaddle;
  leftPaddle.x = 50;
  leftPaddle.y = GetScreenHeight() / 2;
  leftPaddle.width = 10;
  leftPaddle.height = 100;
  leftPaddle.speed = 500;

  Paddle rightPaddle;
  rightPaddle.x = GetScreenWidth() - 50;
  rightPaddle.y = GetScreenHeight() / 2;
  rightPaddle.width = 10;
  rightPaddle.height = 100;
  rightPaddle.speed = 500;

  const char* winnerText = nullptr;
  bool win = false;

  // Collision Sound
  Sound goat = LoadSound("rsc/not_scream.mp3");
  SetSoundVolume(goat, 1.);
  // UnloadSound(goat);
  Sound dramaticMusic = LoadSound("rsc/dramaticMusic.mp3");
  SetSoundVolume(dramaticMusic, 0.5);
  // UnloadSound(dramaticMusic);

  if (intro()) return 0;

  if (controls(std::vector<int>{upLeft, downLeft, upRight, downRight}))
    return 0;
  PlaySound(dramaticMusic);

  CLITERAL(Color) winColor = CLITERAL(Color){0, 0, 0, 255};
  int numFrames = 0;

  while (!WindowShouldClose()) {
    if (!IsSoundPlaying(dramaticMusic)) PlaySound(dramaticMusic);

    ++numFrames;
    if (numFrames % 10 == 0)
      winColor = CLITERAL(Color){(unsigned char)((numFrames + 200) % 255),
                                 (unsigned char)((numFrames + 50) % 255),
                                 (unsigned char)((numFrames + 100) % 255), 255};
    ball.x += ball.speedX * GetFrameTime();
    ball.y += ball.speedY * GetFrameTime();

    if (ball.y < 0) {
      ball.y = 0;
      ball.speedY *= -1;
    }
    if (ball.y > GetScreenHeight()) {
      ball.y = GetScreenHeight();
      ball.speedY *= -1;
    }

    if (IsKeyDown(upLeft)) {
      if (leftPaddle.y > leftPaddle.height / 2)
        leftPaddle.y -= leftPaddle.speed * GetFrameTime();
    }
    if (IsKeyDown(downLeft)) {
      if (leftPaddle.y < GetScreenHeight() - leftPaddle.height / 2)
        leftPaddle.y += leftPaddle.speed * GetFrameTime();
    }

    if (IsKeyDown(upRight)) {
      if (rightPaddle.y > rightPaddle.height / 2)
        rightPaddle.y -= rightPaddle.speed * GetFrameTime();
    }
    if (IsKeyDown(downRight)) {
      if (rightPaddle.y < GetScreenHeight() - rightPaddle.height / 2)
        rightPaddle.y += rightPaddle.speed * GetFrameTime();
    }

    if (IsKeyDown(KEY_Y)) {
      OpenURL(
          "https://www.youtube.com/"
          "watch?v=qLv7LsMDiFs&ab_channel=faginabag420");
      UnloadSound(goat);
      UnloadSound(dramaticMusic);
      unload();
      return 0;
    }

    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                leftPaddle.GetRect())) {
      PlaySound(goat);
      if (ball.speedX < 0) {
        ball.speedX *= -1.1f;
        ball.speedY =
            (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
      }
    }
    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                rightPaddle.GetRect())) {
      PlaySound(goat);
      if (ball.speedX > 0) {
        ball.speedX *= -1.1f;
        ball.speedY =
            (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
      }
    }

    if (ball.x < 0) {
      winnerText = "Left Player Sucks!";
      win = true;
    }
    if (ball.x > GetScreenWidth()) {
      winnerText = "Right Player Sucks!";
      win = true;
    }
    if (winnerText && IsKeyPressed(KEY_SPACE)) {
      ball.x = GetScreenWidth() / 2;
      ball.y = GetScreenHeight() / 2;
      ball.speedX = 300;
      ball.speedY = 300;
      winnerText = nullptr;
      win = false;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    ball.Draw();
    leftPaddle.Draw();
    rightPaddle.Draw();

    if (win) {
      int textWidth = MeasureText(winnerText, 60);
      DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2,
               GetScreenHeight() / 2 - 30, 60, winColor);
    }

    DrawFPS(10, 10);
    EndDrawing();
  }

  UnloadSound(goat);
  UnloadSound(dramaticMusic);
  unload();

  return 0;
}