//Nick Zheng
//Assignment 6 - Asteroids
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
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
	int state;
	bool done;

	float lastFrameTicks;
	float timeLeftOver;
	int hitCount;

	GLuint font;
	GLuint sheetSprite;

	Entity player;
	Entity bullet;
	vector <Entity*> asteroids;
};
