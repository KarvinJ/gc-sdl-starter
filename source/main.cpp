#include <gccore.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "sdl_starter.h"
#include "sdl_assets_loader.h"

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

const int SPEED = 600;

SDL_Rect player = {SCREEN_WIDTH / 2 - 64, SCREEN_HEIGHT / 2 - 64, 64, 64};

void quitGame()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			quitGame();
			exit(0);
		}
	}
}

void update(float deltaTime)
{
	// PAD_ButtonsDown tells us which buttons were pressed in this loop
	// this is a "one shot" state which will not fire again until the button has been released
	const u32 padDown = PAD_ButtonsDown(0);

	// PAD_ButtonsHeld tells us which buttons are keep pressing in this loop
	const u32 padHeld = PAD_ButtonsHeld(0);

	// We return to the launcher application via exit
	if (padDown & PAD_BUTTON_START)
		exit(0);

	if (padHeld & PAD_BUTTON_LEFT && player.x > 0)
	{
		player.x -= SPEED * deltaTime;
	}

	else if (padHeld & PAD_BUTTON_RIGHT && player.x < SCREEN_WIDTH - player.w)
	{
		player.x += SPEED * deltaTime;
	}

	else if (padHeld & PAD_BUTTON_UP && player.y > 0)
	{
		player.y -= SPEED * deltaTime;
	}

	else if (padHeld & PAD_BUTTON_DOWN && player.y < SCREEN_HEIGHT - player.h)
	{
		player.y += SPEED * deltaTime;
	}
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderFillRect(renderer, &player);

	SDL_RenderPresent(renderer);
}

int main(int argc, char **argv)
{
	window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (startSDL(window, renderer) > 0)
	{
		return 1;
	}

	Uint32 previousFrameTime = SDL_GetTicks();
    Uint32 currentFrameTime = previousFrameTime;
    float deltaTime = 0.0f;

	PAD_Init();
	
	while (true)
	{
		PAD_ScanPads();

		currentFrameTime = SDL_GetTicks();
        deltaTime = (currentFrameTime - previousFrameTime) / 1000.0f;
        previousFrameTime = currentFrameTime;

        handleEvents();
        update(deltaTime);
        render();
	}
}