#pragma once
#include "Entity.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class App{
public:
	
	App();
	~App();

	void init();
	bool updateAndRender();
	void render();
	void update();

	void read();
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	bool readEntityData(ifstream& stream);

	void placeEntity(string& type, float placeX, float placeY);

	void makeLevel();

	void renderMainMenu();
	void renderInstrMenu();
	void renderGameLevel();
	void renderGameOver();

	void updateGameLevel();

	void reset();

private:

	SDL_Window* displayWindow;
	const Uint8* keys;
	ifstream inFile;
	/*
	Mix_Chunk* gemSound;
	Mix_Chunk* jumpSound;
	Mix_Chunk* dieSound;
	Mix_Chunk* winSound;
	Mix_Music* music;
	*/
	int state;
	bool done;

	unsigned int font;
	unsigned int characters;
	
	Entity* player;
	/*
	Entity endPoint;
	vector<Entity*> stars;
	vector<Entity*> enemies;
	vector<Entity*> blocks;
	*/

	float gravity;

	unsigned char** levelData;
	int mapWidth;
	int mapHeight;
	
};