//Nick Zheng
//Assignment 3 - Space Invaders

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"
#include <string>
#include <vector>

using namespace std;

class App{
public:

	App();
	~App();

	
	void init();
	void render();
	void update(float elapsed);
	void reset();

	void renderMainMenu();
	void renderGameLevel();
	void renderGameOver();

	void updateGameLevel(float elapsed);

	bool updateAndRender();

private:

	SDL_Window* displayWindow;
	const Uint8* keys;
	bool done;
	int state;
	float lastFrameTicks;

	Entity player;
	Entity bullet;
	vector<Entity*> enemies;
	vector<Entity*> enemyBullets;

	unsigned int font;
	unsigned int sprite;
	
	int score;
};