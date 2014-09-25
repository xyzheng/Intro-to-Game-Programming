//Nick Zheng
//Assignment 2 - Pong

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity{
public:
	Entity(int texture, float xPos, float yPos, float rotate, float w, float h);
	~Entity();

	void draw();

	int textureID;
	float x;
	float y;
	float rotation;
	float width;
	float height;

	float speed;
	float xDirect;
	float yDirect;
};
