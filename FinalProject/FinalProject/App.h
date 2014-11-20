#pragma once
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
	void renderInstrMenu();
	void renderGameLevel();
	void renderGameOver();

	void updateGameLevel();

	void reset();

private:

	SDL_Window* displayWindow;
	const Uint8* keys;

	int state;
	bool done;

	GLuint font;
	GLuint characters;
	GLuint mapSprite;

	vector<Entity*> map;
	vector<Entity*> playerOneUnits;
	vector<Entity*> playerTwoUnits;

	bool selected;

};