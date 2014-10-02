#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "Entity.h"
#include <iostream>

using namespace std;

class App{
public:

	App();
	~App();

	void init();
	void render();
	void update(float elapsed);
	bool updateAndRender();

//	void renderMainMenu();
	void renderGameLevel();
//	void renderGameOver();
//
	void restart();


private:
	
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	const Uint8* keys;

	int score;

	Entity* player;
	vector<Entity> enemiesRowOne;
	vector<Entity> enemiesRowTwo;
	vector<Entity> enemiesRowThree;
	vector<Entity> enemiesRowFour;
	vector<Entity> enemiesRowFive;
	
};