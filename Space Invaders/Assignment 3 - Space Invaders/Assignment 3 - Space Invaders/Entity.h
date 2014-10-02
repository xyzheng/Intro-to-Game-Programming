#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Spritesheet.h"

class Entity{
public:

	Entity(Spritesheet sprite, float scale, float x, float y, float rotation, float xDirection);
	~Entity();

	void render();

	float scale;
	float x;
	float y;
	float rotation;
	float speed;
	float xDirection;
	bool aliveStatus;

	Spritesheet sheetSprite;

};