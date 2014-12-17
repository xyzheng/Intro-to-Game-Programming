//Nick Zheng, Anthony Wong
//Final Project

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "Entity.h"
#include "ParticleEmitter.h"
#include <algorithm>
#include <cstdlib>

using namespace std;

class App{
public:

	App();
	~App();

	void init();
	void update();
	void resetLevelOne();
	void resetLevelTwo();
	void resetLevelThree();

	void render(float elapsed);
	void renderMainMenu();
	void renderInstrMenu();
	void renderLevelOne(float elapsed);
	void renderLevelTwo(float elapsed);
	void renderLevelThree(float elapsed);
	void renderGameOver();

	void updateGameLevelOne();
	void updateGameLevelTwo();
	void updateGameLevelThree();

	bool readHeader();
	bool readLayerData();
	bool readEntityData();

	bool updateAndRender();

	void placeEntity(string& type, float placeX, float placeY);

	void readLevelOne();
	void readLevelTwo();
	void readLevelThree();
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	bool readEntityData(ifstream& stream);

	void clearLevel();
	void makeLevel();

	void worldToTileCoord(float worldX, float worldY, int* gridX, int* gridY);
	bool isSolid(int tile);
	float getCollideX(float x, float y);
	float getCollideY(float x, float y);
	void collideWithMapX(Entity* ent);
	void collideWithMapY(Entity* ent);

private:

	SDL_Window* displayWindow;
	const Uint8* keys;
	bool done;
	ifstream inFile;

	Mix_Chunk* gem;
    Mix_Chunk* jump;
    Mix_Chunk* next;
    Mix_Chunk* spike;
    Mix_Chunk* lavas;
    Mix_Music* menu;

	ParticleEmitter* emitterOne;
	ParticleEmitter* emitterTwo;

	int state;
	float gravity;

	GLuint font;
	GLuint sheetSprite;
	GLuint characters;

	unsigned char** levelData;

	int mapWidth;
	int mapHeight;
	
	Entity* playerOne;
	Entity* playerTwo;

	//stage 1
	vector<Entity*> spikes;
	vector<Entity*> blueGems;
	vector<Entity*> greenGems;
	vector<Entity*> exitKeys;

	Entity* exitTop;
	Entity* exitBot;

	//stage 2
	vector<Entity*> lava;

	//stage 3 stuff
	vector<Entity*> exitOne;
	vector<Entity*> exitTwo;

};