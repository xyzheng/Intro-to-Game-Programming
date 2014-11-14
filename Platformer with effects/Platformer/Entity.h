//Nick Zheng
//Assignment 4 - Single Screen Platformer WITH SOUND
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity{
public:

	Entity(unsigned int spritesheet, float x, float y, float xD, float yD, float rotate, float u, 
		float v, float width, float height, float mass, bool dynamic, bool collide);
	Entity(){};

	//draw sprite
	void drawSprite(float scale);

	//check if entity collides in x or y direction
	bool collideX(Entity* entity);
	bool collideY(Entity* entity);

	unsigned int textureID;

	float x;
	float y;
	float xD;
	float yD;
	float rotation;
	float u;
	float v;
	float width;
	float height;
	
	float mass;
	float xVelo;
	float yVelo;
	float xAcc;
	float yAcc;
	float xFric;
	float yFric;

	bool visible;
	bool dynamic;
	bool collide;

	bool collidedTop;
	bool collidedBot;
	bool collidedRight;
	bool collidedLeft;
	

};