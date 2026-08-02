#include <stdio.h>
#include "raylib.h"

#define WIDTH  800
#define HEIGHT 600
#define FONT_SIZE 20
#define GRID_SIZE 12
#define TICK_RATE 0.15f /* 150ms to wait before moving */

typedef struct Player {
	Vector2 position;
	Vector2 direction;
	Rectangle icon;
} Player;

void handle_input(Player *player);
void update_player(Player *player, float deltaTime, float *moveTimer);
void draw_debug_info(const Player *player);
void draw_player(Player *player);

int main(void)
{
	int width = WIDTH;
	int height = HEIGHT;

	InitWindow(width, height, "Snakey: a snake clone with basic networking functionality!");
	SetTargetFPS(60);

	Player player = {
		.position = {(width / 2.0f), (height / 2.0f)},
		.direction = {0, 0},
		.icon = {player.position.x, player.position.y, GRID_SIZE, GRID_SIZE}
	};

	if (!IsWindowReady())
		return -1;

	float moveTimer = 0.0f;

	while(!WindowShouldClose()) {
		float deltaTime = GetFrameTime();
		handle_input(&player);
		update_player(&player, deltaTime, &moveTimer);
		BeginDrawing();
		ClearBackground(WHITE);
		draw_debug_info(&player);
		draw_player(&player);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

void handle_input(Player *player)
{
	if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && player->direction.y == 0)
		player->direction = (Vector2){0, -1};

	if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && player->direction.y == 0)
		player->direction = (Vector2){0, 1};

	if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && player->direction.x == 0)
		player->direction = (Vector2){-1, 0};

	if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && player->direction.x == 0)
		player->direction = (Vector2){1, 0};
}

void draw_debug_info(const Player *player)
{
	DrawText(TextFormat("Position: %f, %f", player->position.x, player->position.y), 0, 0, FONT_SIZE, RED);
	DrawText(TextFormat("Direction: %f, %f", player->direction.x, player->direction.y), 0, 20, FONT_SIZE, RED);
}

void update_player(Player *player, float deltaTime, float *moveTimer)
{
	*moveTimer += deltaTime;

	if (*moveTimer >= TICK_RATE) {
		player->position.x += player->direction.x * GRID_SIZE;
		player->position.y += player->direction.y * GRID_SIZE;

		*moveTimer -= TICK_RATE;
	}


}

void draw_player(Player *player)
{
	player->icon.x = player->position.x;
	player->icon.y = player->position.y;

	DrawRectangleRec(player->icon, RED);
}
