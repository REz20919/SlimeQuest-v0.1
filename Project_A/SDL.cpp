#include "SDL_header.h"
#include "C++.h"
//Screen dimension constants
const int SCREEN_WIDTH = 629;
 const int SCREEN_HEIGHT = 476;

//Quit flag
bool quit = false;

//Player sprite
SDL_Surface* player_sprite;

SDL_Surface* background;

//The window we'll be rendering to
SDL_Window* window = NULL;


//The surface contained by the window
SDL_Surface* screenSurface = NULL;
SDL_Rect player_loc;

//Setting up SDL
int SDL_Init() {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
}

//Declaring player location
void player_loc_init() {
	player_loc.x = 1;
	player_loc.y = 1;
	player_loc.w = 37;
	player_loc.h = 28;
}

//Load function
SDL_Surface* loadSurface(std::string path) {
	//Loads an image
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());

	//Checking for error
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	return loadedSurface;
}

//Loading sprites and backgrounds
bool loadMedia() {
	bool success = true;
	player_sprite = loadSurface("player_sprite_1.bmp");
	if (player_sprite == NULL) {
		printf("Failed to load player sprite!\n");
		success = false;
	}
	background = loadSurface("background.bmp");
	if (background == NULL) {
		printf("Failed to load player sprite!\n");
		success = false;
	}
	return success;
}

//Limiting to one input at a time
bool Key_Restart = 1;
void Key_Restart_Check(SDL_Event &e) {
	int keys = 0;
	if (e.type == SDL_KEYUP) {
		switch (e.key.keysym.sym) {
		case SDLK_UP:
			Key_Restart = 1;
			break;
		case SDLK_DOWN:
			Key_Restart = 1;
			break;
		case SDLK_LEFT:
			Key_Restart = 1;
			break;
		case SDLK_RIGHT:
			Key_Restart = 1;
			break;

		}
	}

}

//events
void events(SDL_Event &e) {
	while (SDL_PollEvent(&e) != 0) {
		//User requests quit
		if (e.type == SDL_QUIT)
			quit = true;
		//User presses a key
		Key_Restart_Check(e);
		if (e.type == SDL_KEYDOWN && Key_Restart == 1) {
			//key inputs
			switch (e.key.keysym.sym) {
			case SDLK_UP:
				if (player_loc.y > 28 * 2)
					player_loc.y -= 28 * 2;
				Key_Restart = 0;
				break;

			case SDLK_DOWN:
				if (player_loc.y < SCREEN_HEIGHT - 28 * 2)
					player_loc.y += 28 * 2;
				Key_Restart = 0;
				break;

			case SDLK_LEFT:
				if (player_loc.x > 37 * 2)
					player_loc.x -= 37 * 2;
				Key_Restart = 0;
				break;

			case SDLK_RIGHT:
				if (player_loc.x < SCREEN_WIDTH - 37 * 2)
					player_loc.x += 37 * 2;
				Key_Restart = 0;
				break;
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
		}
		//Clearing the screen
		SDL_BlitScaled(background, NULL, screenSurface, NULL);
		system("cls");
		printf("x:%d ,y:%d\n", player_loc.x, player_loc.y);

	}
}