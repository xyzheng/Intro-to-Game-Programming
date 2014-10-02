#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"
#include "Bullet.h"
#include "Spritesheet.h"
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

using namespace std;

class App{
public:

	App();
	~App();

	void init();
	void render();
	void update(float elapsed);

	void renderMainMenu();
	void renderGameLevel();
	void renderGameOver();

	void updateMainMenu(float elapsed);
	void updateGameLevel(float elapsed);
	void updateGameOver(float elapsed);

	void drawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);
	bool updateAndRender();

	void shoot();

	
private:

	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	vector<Entity*> rowOne;
	vector<Entity*> rowTwo;
	vector<Entity*> rowThree;
	vector<Entity*> rowFour;

	GLuint textSheet;
	GLuint spriteSheet;
	Entity* player;
	Bullet playerShots;
	Bullet enemyShots[4];

	int score;
	float delay;

};