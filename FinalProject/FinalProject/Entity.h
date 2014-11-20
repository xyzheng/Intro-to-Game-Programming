#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity{
public:

	Entity(unsigned int spriteSheet, float x, float y, float rotate, float width, float height);
	Entity(){};

	//draw sprite
	void draw(int spriteTexture, int index, int spriteCountX, int spriteCountY);

	//check if entity collides in x or y direction
//	bool collideX(Entity* entity);
//	bool collideY(Entity* entity);

	unsigned int textureID;

	float x;
	float y;
	float rotation;
	float width;
	float height;

	//stats
	float attack;
	float defense;
	
	bool visible;
	bool dynamic;
	bool collide;

	bool collidedTop;
	bool collidedBot;
	bool collidedRight;
	bool collidedLeft;
	
};