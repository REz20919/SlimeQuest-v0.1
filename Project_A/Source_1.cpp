//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <time.h>
//Screen dimension constants
const int SCREEN_WIDTH = 624;
const int SCREEN_HEIGHT = 624;




int Map[8][8], nr_mobi = 1;
//Quit flag
bool quit = false;
bool died = false;
//Player sprite
unsigned int score = 0, turn = 0, highscore = 0;
char s[100];

SDL_Surface* player_sprite;

SDL_Surface* enemy_sprite;

SDL_Surface* enemy_sprite_sb;

SDL_Surface* background;

//The window we'll be rendering to
SDL_Window* window = NULL;


//The surface contained by the window
SDL_Surface* screenSurface = NULL;
SDL_Rect player_loc, enemy_loc;

//Setting up SDL
int SDL_Init() {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	window = SDL_CreateWindow("Atestat", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}
struct axis {
	int x;
	int y;
}movement, player, spawn_location[40];
struct enemy {
	int x;
	int y;
	bool move = true;
}mobs[65];

//Declaring player location
void sprite_loc_init() {
	//Player start data
	player.x = 0;
	player.y = 0;
	player_loc.x = 0;
	player_loc.y = 0;
	player_loc.w = 64;
	player_loc.h = 64;
	Map[0][0] = 1;
	//Mobs start data
	mobs[0].x = 3;
	mobs[0].y = 3;
	//mobs[1].x = 7;
	//mobs[1].y = 7;
	enemy_loc.x = mobs[0].x*(player_loc.w + 16);
	enemy_loc.y = mobs[0].y*(player_loc.h + 16);
	enemy_loc.h = 64;
	enemy_loc.w = 64;
	Map[mobs[0].x][mobs[0].y] = 2;
	//Movement data
	movement.x = player_loc.w;
	movement.y = player_loc.h;
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
	background = loadSurface("background_1.bmp");
	if (background == NULL) {
		printf("Failed to load player sprite!\n");
		success = false;
	}
	enemy_sprite = loadSurface("enemy_1.bmp");
	if (enemy_sprite == NULL) {
		printf("Failed to load player sprite!\n");
		success = false;
	}
	enemy_sprite_sb = loadSurface("enemy_2.bmp");
	return success;
}
/*void checkscore() {
if (score > highscore) {
system("cls");
highscore = score;
printf("Congratulations enter your name here:");
fgets(s, sizeof(s), stdin);
system("cls");
printf("New Highscore Owner:%s\n New HighScore=%d", s, highscore);
}
else {
system("cls");
printf("Too bad try again\n HighScore Owner:%s\n Highscore=%d\n");
}
}*/
void restart() {
	turn = 0;
	score = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			Map[i][j] = 0;
	player.x = 0;
	player.y = 0;
	Map[0][0] = 1;
	Map[3][3] = 2;
	mobs[0].x = 3;
	mobs[0].y = 3;
	mobs[0].move = true;
	nr_mobi = 1;
	died = false;

}
int spawn_locations() {

	int k = 0;
	for (int i = 0; i < 8; i++) {
		if (Map[0][i] == 0) {
			spawn_location[k].x = i;
			spawn_location[k].y = 0;
			k++;
		}
	}
	for (int i = 1; i < 8; i++) {
		if (Map[i][0] == 0) {
			spawn_location[k].x = 0;
			spawn_location[k].y = i;
			k++;
		}
	}
	for (int i = 1; i < 8; i++) {
		if (Map[i][7] == 0) {
			spawn_location[k].x = 7;
			spawn_location[k].y = i;
			k++;
		}
	}
	for (int i = 1; i < 7; i++) {
		if (Map[7][i] == 0) {
			spawn_location[k].x = i;
			spawn_location[k].y = 7;
			k++;
		}
	}


	for (int i = 0; i < k; i++)
		printf("\nSpawn Location: x=%d y=%d\n", spawn_location[i].x, spawn_location[i].y);
	//printf("\nk=%d\n", k);
	return k;
}
int RNG(int n) {
	//printf("\nn=%d\n", n);
	int x = rand() % n;
	return x;
}
void spawnmob() {
	int x = RNG(spawn_locations());
	mobs[nr_mobi].y = spawn_location[x].y;
	mobs[nr_mobi].x = spawn_location[x].x;
	Map[mobs[nr_mobi].y][mobs[nr_mobi].x] = 2;
	mobs[nr_mobi].move = false;
	nr_mobi++;

}
//enemy AI
void enemy_movement(int i) {

	if (mobs[i].move == true) {
		mobs[i].move = false;
		if (Map[mobs[i].y][mobs[i].x] == 2) {
			Map[mobs[i].y][mobs[i].x] = 0;
			if ((abs(mobs[i].x - player.x) > abs(mobs[i].y - player.y) || mobs[i].y != player.y) && mobs[i].x != player.x && (Map[mobs[i].y][mobs[i].x + 1] != 2 || Map[mobs[i].y][mobs[i].x - 1] != 2)) {
				if (mobs[i].x < player.x && Map[mobs[i].y][mobs[i].x + 1] != 2) mobs[i].x++;
				if (mobs[i].x > player.x && Map[mobs[i].y][mobs[i].x - 1] != 2) mobs[i].x--;
			}
			else if (Map[mobs[i].y + 1][mobs[i].x] != 2 || Map[mobs[i].y - 1][mobs[i].x] != 2) {
				if (mobs[i].y < player.y && Map[mobs[i].y + 1][mobs[i].x] != 2) mobs[i].y++;
				if (mobs[i].y > player.y && Map[mobs[i].y - 1][mobs[i].x] != 2) mobs[i].y--;
			}
			Map[mobs[i].y][mobs[i].x] = 2;
			return;
		}
	}
	else mobs[i].move = true;
	if (Map[mobs[i].y][mobs[i].x] != 2) {
		mobs[i].x = mobs[nr_mobi - 1].x;
		mobs[i].y = mobs[nr_mobi - 1].y;
		nr_mobi--;
	}
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
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
			restart();
		if (e.type == SDL_KEYDOWN && Key_Restart == 1 && died == false) {
			//key inputs

			switch (e.key.keysym.sym) {
			case SDLK_UP:
				if (player.y > 0) {
					Map[player.y][player.x] = 0;
					player.y--;
					if (Map[player.y][player.x] == 2) {
						score += 50;
					}
					else score += 10;
					Map[player.y][player.x] = 1;
				}
				turn++;
				SDL_BlitScaled(background, NULL, screenSurface, NULL);
				for (int i = 0; i<nr_mobi; i++)
					enemy_movement(i);
				spawnmob();
				player_loc.x = player.x*(movement.x + 16);
				player_loc.y = player.y*(movement.y + 16);
				Key_Restart = 0;
				break;

			case SDLK_DOWN:
				if (player.y < 7) {
					Map[player.y][player.x] = 0;
					player.y++;
					if (Map[player.y][player.x] == 2) {
						score += 50;
					}
					else score += 10;
					Map[player.y][player.x] = 1;

				}
				turn++;
				SDL_BlitScaled(background, NULL, screenSurface, NULL);
				for (int i = 0; i<nr_mobi; i++)
					enemy_movement(i);
				spawnmob();
				player_loc.x = player.x*(movement.x + 16);
				player_loc.y = player.y*(movement.y + 16);
				Key_Restart = 0;
				break;

			case SDLK_LEFT:
				if (player.x > 0) {
					Map[player.y][player.x] = 0;
					player.x--;
					if (Map[player.y][player.x] == 2) {
						score += 50;
					}
					else score += 10;
					Map[player.y][player.x] = 1;

				}
				turn++;
				SDL_BlitScaled(background, NULL, screenSurface, NULL);
				for (int i = 0; i<nr_mobi; i++)
					enemy_movement(i);
				spawnmob();
				player_loc.x = player.x*(movement.x + 16);
				player_loc.y = player.y*(movement.y + 16);
				Key_Restart = 0;
				break;

			case SDLK_RIGHT:
				if (player.x < 7) {
					Map[player.y][player.x] = 0;
					player.x++;
					if (Map[player.y][player.x] == 2) {
						score += 50;
					}
					else score += 10;
					Map[player.y][player.x] = 1;
				}
				turn++;
				SDL_BlitScaled(background, NULL, screenSurface, NULL);
				for (int i = 0; i<nr_mobi; i++)
					enemy_movement(i);
				spawnmob();
				player_loc.x = player.x*(movement.x + 16);
				player_loc.y = player.y*(movement.y + 16);
				Key_Restart = 0;
				break;
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
			SDL_BlitScaled(background, NULL, screenSurface, NULL);
			player_loc.x = player.x*(movement.x + 16);
			player_loc.y = player.y*(movement.y + 16);
			//Clearing the screen

			//		enemy_loc.x = mobs[0].x*(movement.x + 16);
			//	enemy_loc.y = mobs[0].y*(movement.y + 16);
			//Debugging
			system("cls");
			printf("player x:%d ,y:%d\n x:%d ,y:%d\n", player_loc.x, player_loc.y, player.x, player.y);
			//printf("Rand:%d\nNr mobi:%d\n", RNG(spawn_locations()), nr_mobi);
			for (int i = 0; i<nr_mobi; i++)
				printf("mob x:%d ,y:%d\n x:%d ,y:%d,move:%d\n", mobs[i].x*(movement.x + 16), mobs[i].y*(movement.y + 16), mobs[i].x, mobs[i].y, mobs[i].move);
			for (int i = 0; i <= 7; i++) {
				printf("\n");
				for (int j = 0; j <= 7; j++)
					printf("%d ", Map[i][j]);
			}
			printf("\nSCORE:%d", score);
		}


	}
}



int main(int argc, char* args[])
{

	//Declaring event variable
	SDL_Event e;
	srand(time(NULL));
	//Initializing player location
	sprite_loc_init();
	loadMedia();
	int i = 0;
	if (!SDL_Init()) return 0;
	printf("player x:%d ,y:%d\n x:%d ,y:%d\n", player_loc.x, player_loc.y, player.x, player.y);
	printf("Nr mobi:%d\n", nr_mobi);
	for (int i = 0; i<nr_mobi; i++)
		printf("mob x:%d ,y:%d\n x:%d ,y:%d,move:%d\n", enemy_loc.x, enemy_loc.y, mobs[i].x, mobs[i].y, mobs[i].move);
	for (int i = 0; i <= 7; i++) {
		printf("\n");
		for (int j = 0; j <= 7; j++)
			printf("%d ", Map[i][j]);
	}
	printf("\nSCORE:%d", score);
	screenSurface = SDL_GetWindowSurface(window);
	SDL_BlitScaled(background, NULL, screenSurface, NULL);
	//Game loop
	while (quit != true) {
		//Get window surface
		screenSurface = SDL_GetWindowSurface(window);
		if (Map[player.y][player.x] != 1) {
			died = true;
			//checkscore();
		}
		//Processing the events
		events(e);

		//Apply player sprite
		SDL_BlitScaled(player_sprite, NULL, screenSurface, &player_loc);


		//Apply the mob sprite
		for (int i = 0; i < nr_mobi; i++) {
			enemy_loc.x = mobs[i].x*(movement.x + 16);
			enemy_loc.y = mobs[i].y*(movement.y + 16);
			if (mobs[i].move == true)
				SDL_BlitScaled(enemy_sprite, NULL, screenSurface, &enemy_loc);
			else SDL_BlitScaled(enemy_sprite_sb, NULL, screenSurface, &enemy_loc);
		}

		//Update the surface
		SDL_UpdateWindowSurface(window);

	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}