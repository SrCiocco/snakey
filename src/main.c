#include <stdio.h>
#include "raylib.h"

#define WIDTH  640
#define HEIGHT 480

int main(void)
{
	int width = WIDTH;
	int height = HEIGHT;

	InitWindow(width, height, "Snakey: a snake clone with basic networking functionality!");
	SetTargetFPS(60);

	if (!IsWindowReady())
		return -1;

	while(!WindowShouldClose()) {
		BeginDrawing();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
