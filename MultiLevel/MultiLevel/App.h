//Nick Zheng
//Assignment 5 - Side Scroller

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "Entity.h"

using namespace std;

class App{
public:

	App();
	~App();

	void init();
	void update();
	void reset();

	void render();
	void renderMainMenu();
	void renderGameLevel();
	void renderGameOver();

	void updateGameLevel();

	void readFile();
	bool readHeader();
	bool readLayerData();
	bool readEntityData();

	bool updateAndRender();

	void placeEntity(string& type, float placeX, float placeY);

	void read();
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	bool readEntityData(ifstream& stream);

	void clearLevel();
	void makeLevel();

	void worldToTileCoord(float worldX, float worldY, int* gridX, int* gridY);
	float getCollideX(float x, float y);
	float getCollideY(float x, float y);
	void collideWithMapX(Entity* ent);
	void collideWithMapY(Entity* ent);

private:

	SDL_Window* displayWindow;
	const Uint8* keys;
	bool done;
	ifstream inFile;

	int state;
	int score;
	float gravity;

	GLuint font;
	GLuint sheetSprite;
	GLuint characters;

	unsigned char** levelData;
	int mapWidth;
	int mapHeight;

	vector<Entity*> enemies;
	vector<Entity*> gems;
	Entity* player;

};