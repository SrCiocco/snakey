#include <stdio.h>
#include "raylib.h"

#define WIDTH  640
#define HEIGHT 480
#define FONT_SIZE 20
#define GRID_SIZE 20
#define TICK_RATE 0.10f /* 100ms to wait before moving */
#define MAX_SNAKE_LENGTH 256

typedef struct Player {
	Vector2 body[MAX_SNAKE_LENGTH];
	int actualLength;
	Vector2 direction;
	Rectangle icon;
	bool alive;
} Player;

enum {
	HEAD = 0
};

typedef struct Food {
	Vector2 position;
	Rectangle icon;
	bool canSpawn;
} Food;

void handle_input(Player *player);
void update_player(Player *player, float deltaTime, float *moveTimer);
void draw_debug_info(const Player *player, const Food *food);
void draw_player(const Player *player);
void player_setup(Player *player);
void spawn_food(Food *food);
void setup_food(Food *food);
void draw_food(Food *food);
void update_game_state(Player *player, Food *food);
void draw_game_over();
void game_loop(Player *player, Food *food);

int main(void)
{

	Player player = {0};

	Food food = {0};

	game_loop(&player, &food);

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

void draw_debug_info(const Player *player, const Food *food)
{
	DrawText(TextFormat("Position: %.0f, %.0f", player->body[HEAD].x, player->body[HEAD].y), 0, 0, FONT_SIZE, RED);
	DrawText(TextFormat("Direction: %.0f, %.0f", player->direction.x, player->direction.y), 0, 20, FONT_SIZE, RED);
	DrawText(TextFormat("Length: %d", player->actualLength), 0, 40, FONT_SIZE, RED);
	DrawText(TextFormat("Food position: %.0f, %.0f", food->position.x, food->position.y), 0, 60, FONT_SIZE, RED);

}

void update_player(Player *player, float deltaTime, float *moveTimer)
{
	*moveTimer += deltaTime;

	if (*moveTimer >= TICK_RATE) {
		for (int i = player->actualLength - 1; i > 0; i--)
			player->body[i] = player->body[i - 1];

		player->body[HEAD].x += player->direction.x * GRID_SIZE;
		player->body[HEAD].y += player->direction.y * GRID_SIZE;

		player->icon.x = player->body[HEAD].x;
		player->icon.y = player->body[HEAD].y;

		*moveTimer -= TICK_RATE;
	}
}

void draw_player(const Player *player)
{
	for (int i = 0; i < player->actualLength; i++) {
		Color color = (i == 0) ? MAROON: GREEN; /* Head will be different from the body */
		DrawRectangle(player->body[i].x, player->body[i].y, GRID_SIZE, GRID_SIZE, color);
	}
}


void player_setup(Player *player)
{
	int x = GetScreenWidth() / 2;
	int y = GetScreenHeight() / 2;

	player->actualLength = 1;
	player->body[HEAD].x = (float) ((x / GRID_SIZE) * GRID_SIZE); /* Truncate position to grid cell and cast to float for Raylib's sake */
	player->body[HEAD].y = (float) ((y / GRID_SIZE) * GRID_SIZE);
	player->direction.x = 0.0f;
	player->direction.y = 0.0f;
	player->icon.x = player->body[HEAD].x;
	player->icon.y = player->body[HEAD].y;
	player->icon.width = GRID_SIZE;
	player->icon.height = GRID_SIZE;
	player->alive = true;
}

void spawn_food(Food *food)
{
	if (!food->canSpawn)
		return;

	int x = GetScreenWidth() / GRID_SIZE;
	int y = GetScreenHeight() / GRID_SIZE;

	food->position.x = (float) (GetRandomValue(x / 2, x - 1) * GRID_SIZE);
	food->position.y = (float) (GetRandomValue(y / 2, y - 1) * GRID_SIZE);
	food->canSpawn = false;
}

void setup_food(Food *food)
{
	food->canSpawn = true;
	food->icon.width = GRID_SIZE;
	food->icon.height = GRID_SIZE;
}

void draw_food(Food *food)
{
	food->icon.x = food->position.x;
	food->icon.y = food->position.y;

	DrawRectangleRec(food->icon, RED);
}

void update_game_state(Player *player, Food *food)
{
	if (food->position.x == player->body[HEAD].x && food->position.y == player->body[HEAD].y) {
		food->canSpawn = true;
		if (player->actualLength < MAX_SNAKE_LENGTH) {
			player->actualLength++;
		}
	}

	for (int i = 1; i < player->actualLength; i++) {
		if ((player->body[HEAD].x == player->body[i].x) && (player->body[HEAD].y == player->body[i].y)) {
			player->alive = false;
		}
	}
}

void draw_game_over()
{
	const int width = (GetScreenWidth() / 2);
	const int height = (GetScreenHeight() / 2);
	DrawText("Wanna try again?", width, height, FONT_SIZE, RED);
	DrawText("(Y)es or (N)o", width, height + 20, FONT_SIZE, RED);
}

void game_loop(Player *player, Food *food)
{

	int width = WIDTH;
	int height = HEIGHT;

	InitWindow(width, height, "Snakey: a snake clone with basic networking functionality!");
	SetTargetFPS(60);

	if (!IsWindowReady())
		return;

	float moveTimer = 0.0f;

	bool exitGame = false;

	player_setup(player);
	setup_food(food);

	while (!WindowShouldClose() && !exitGame) {
		float deltaTime = GetFrameTime();
		if (player->alive) {
			handle_input(player);
			update_player(player, deltaTime, &moveTimer);
			spawn_food(food);
			update_game_state(player, food);
		}
		else {
			if (IsKeyPressed(KEY_Y)) {
				player_setup(player);
				setup_food(food);
				moveTimer = 0.0f;
			}
			else if (IsKeyPressed(KEY_N)) {
				exitGame = true;
			}
		}
		BeginDrawing();
		ClearBackground(WHITE);
		if (player->alive) {
			draw_debug_info(player, food);
			draw_player(player);
			draw_food(food);
		}
		else {
			draw_game_over();
		}
		EndDrawing();
	}
	CloseWindow();
}
