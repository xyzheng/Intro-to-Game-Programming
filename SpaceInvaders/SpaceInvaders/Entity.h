//Nick Zheng
//Assignment 3 - Space Invaders

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity{
public:

	Entity(unsigned int spritesheet, float x, float y, float xD, float yD, float rotate, float u, float v, float width, float height);
	Entity(){};

	void drawSprite(float scale);
	bool collide(Entity* entity);

	unsigned int textureID;

	float scale;
	float x;
	float y;
	float u;
	float v;
	float width;
	float height;
	float rotation;
	float xDirection;
	float yDirection;
	float speed;

	bool alive;
	bool visible;

};