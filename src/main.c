#include <stdio.h>
#include "raylib.h"

#define WIDTH  800
#define HEIGHT 600
#define FONT_SIZE 20

typedef struct Player {
	Vector2 position;
	Vector2 direction;
} Player;

void handle_input(Player *player);
void update_player(Player *player);
void draw_debug_info(const Player *player);

int main(void)
{
	int width = WIDTH;
	int height = HEIGHT;

	InitWindow(width, height, "Snakey: a snake clone with basic networking functionality!");
	SetTargetFPS(60);

	Player player = {
		.position = {(width / 2.0f), (height / 2.0f)},
		.direction = {0, 0}
	};

	if (!IsWindowReady())
		return -1;

	while(!WindowShouldClose()) {
		handle_input(&player);
		update_player(&player);
		BeginDrawing();
		ClearBackground(WHITE);
		draw_debug_info(&player);
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

void update_player(Player *player)
{
	player->position.x += player->direction.x;
	player->position.y += player->direction.y;
	
}
