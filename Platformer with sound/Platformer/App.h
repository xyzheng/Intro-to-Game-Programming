//Nick Zheng
//Assignment 4 - Single Screen Platformer WITH SOUND
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Entity.h"
#include <vector>
#include <string>

using namespace std;

class App{
public:
	
	App();
	~App();

	void init();
	bool updateAndRender();
	void render();
	void update();

	void renderMainMenu();
	void renderGameLevel();
	void renderGameOver();

	void updateGameLevel();

	void reset();

private:

	SDL_Window* displayWindow;
	const Uint8* keys;
	Mix_Chunk* gemSound;
	Mix_Chunk* jumpSound;
	Mix_Chunk* dieSound;
	Mix_Chunk* winSound;
	Mix_Music* music;
	int state;
	bool done;

	GLuint font;

	Entity player;
	Entity endPoint;
	vector<Entity*> stars;
	vector<Entity*> enemies;
	vector<Entity*> blocks;

	float gravity;
};
