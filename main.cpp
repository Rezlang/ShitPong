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

int intro();
int controls(std::vector<int> keys);

void unload() {
  CloseAudioDevice();
  CloseWindow();
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