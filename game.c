#define RAYMATH_IMPLEMENTATION
#include "raylib.h"
#include "raymath.h"

typedef enum PlayerAnimationState
{
  IDLE,
  WALK,
} PlayerAnimationState;

typedef struct Animation
{
  int currentFrame;
  int framesCounter;
  int framesSpeed;
} Animation;

typedef struct Player
{
  Vector2 pos;
  Vector2 velocity;
  int size;
  float speed;
  PlayerAnimationState animationState;
  Rectangle frameRect;
  Animation animation;
  Vector2 direction;
  Texture2D frontTexture;
  Texture2D backTexture;
  Texture2D sideTexture;
} Player;

typedef struct GameState
{
  Player player;
  Texture2D playerTexture;
} GameState;

void PlayAnimation(Animation *animation, Rectangle *frameRect, int textureWidth)
{
  animation->framesCounter++;

  if (animation->framesCounter >= (60 / animation->framesSpeed))
  {
    animation->framesCounter = 0;
    animation->currentFrame++;

    if (animation->currentFrame > 5)
      animation->currentFrame = 0;

    frameRect->x = (float)animation->currentFrame * (float)textureWidth / 6;
  }
}

int main(void)
{
  InitWindow(1280, 720, "Untitled Wave Game");
  SetTargetFPS(60);

  GameState gameState = {
      .player = {
          .velocity = Vector2Zero(),
          // TODO: temporarily value
          .size = 256,
          .speed = 120.0f,
          .animationState = IDLE,
          .frameRect = {
              .width = 64,
              .height = 64,
          },
          .animation = {
              .framesSpeed = 8,
          },
          .direction = {
              .x = 1,
              .y = 1,
          },
          .frontTexture = LoadTexture("assets/Front/Walk.png"),
          .backTexture = LoadTexture("assets/Back/Walk.png"),
          .sideTexture = LoadTexture("assets/Side/Walk.png"),
      },
  };

  // Set default player texture
  gameState.playerTexture = gameState.player.frontTexture;

  while (!WindowShouldClose())
  {
    // Update player
    {
      if (IsKeyDown(KEY_A))
      {
        gameState.player.direction.x = -1;
        gameState.playerTexture = gameState.player.sideTexture;
        gameState.player.velocity.x -= 1.0f;
        gameState.player.animationState = WALK;
        gameState.player.pos.x -= gameState.player.speed * GetFrameTime();
      }
      if (IsKeyDown(KEY_D))
      {
        gameState.player.direction.x = 1;
        gameState.playerTexture = gameState.player.sideTexture;
        gameState.player.velocity.x += 1.0f;
        gameState.player.animationState = WALK;
        gameState.player.pos.x += gameState.player.speed * GetFrameTime();
      }

      if (IsKeyDown(KEY_W))
      {
        gameState.player.direction.y = -1;
        gameState.playerTexture = gameState.player.backTexture;
        gameState.player.velocity.y -= 1.0f;
        gameState.player.animationState = WALK;
        gameState.player.pos.y -= gameState.player.speed * GetFrameTime();
      }

      if (IsKeyDown(KEY_S))
      {
        gameState.player.direction.y = 1;
        gameState.playerTexture = gameState.player.frontTexture;
        gameState.player.velocity.y += 1.0f;
        gameState.player.animationState = WALK;
        gameState.player.pos.y += gameState.player.speed * GetFrameTime();
      }

      // gameState.player.velocity = Vector2Zero();

      if (gameState.player.animationState == WALK)
      {
        PlayAnimation(&gameState.player.animation, &gameState.player.frameRect, gameState.playerTexture.width);
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);

    // Draw player
    {
      Rectangle playerSource = {
          .x = gameState.player.frameRect.x,
          .y = gameState.player.frameRect.y,
          // Depends on the direction.x to either flip player sprite to left or right
          .width = gameState.player.direction.x * gameState.player.frameRect.width,
          .height = gameState.player.frameRect.height,
      };

      Rectangle playerDest = {
          .x = gameState.player.pos.x,
          .y = gameState.player.pos.y,
          .width = gameState.player.size,
          .height = gameState.player.size,
      };

      DrawTexturePro(gameState.playerTexture,
                     playerSource,
                     playerDest,
                     (Vector2){0, 0},
                     0,
                     WHITE);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
