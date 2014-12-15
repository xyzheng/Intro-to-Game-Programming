//Nick Zheng
//Assignment 5 - Side Scroller

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity{
public:
	Entity(unsigned int spritesheet, float x, float y, float xD, float yD, float rotate, float u, 
	float v, float width, float height, float mass, bool dynamic);
	~Entity();

	void draw(int spriteTexture, int index, int spriteCountX, int spriteCountY);
	bool Entity::collideX(Entity* entity);
	bool Entity::collideY(Entity* entity);

	unsigned int textureID;

	float x;
	float y;
	float xD;
	float yD;
	float u;
	float v;
	float rotation;
	float width;
	float height;

	float mass;
	float xVelo;
	float yVelo;
	float xAcc;
	float yAcc;
	float xFric;
	float yFric;
	
	float dynamic;
	float collidedTop;
	float collidedBot;
	float collidedRight;
	float collidedLeft;

	bool visible;

//	int gridX;
//	int gridY;
};