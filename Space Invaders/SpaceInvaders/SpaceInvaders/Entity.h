#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Bullet.h"

class Entity{
public:

	Entity(GLuint textureID, float u, float v, float width, float height, float scale);
	~Entity();

	void draw();
	void render();

	GLuint textureID;
	float u;
	float v;
	float width;
	float height;
	float scale;
	float xDirect;
	float yDirect;

	float x;
	float y;

//	Bullet missle;

	bool aliveStatus;
	bool shoot;

};