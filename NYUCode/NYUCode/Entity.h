//Nick Zheng
//Assignment 6 - Asteroids
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include <math.h>
#include <algorithm>

class Entity{
public:

	Entity(){};
	Entity(unsigned int textureID, float u, float v, float width, float height, float xScale, float yScale);

	void draw();
	void makeMatrix();
	void update();
	void generateRandom();

	bool collided(Entity* e);

	Matrix matrix;
	Matrix inverse;
	Vector vector;

	unsigned int textureID;

	float x;
	float y;

	float u;
	float v;
	float width;
	float height;
	float scale_x;
	float scale_y;
	float rotation;
	float speed;

	float xScale;
	float yScale;

	//collision stuff
	Vector topLeft;
	Vector topRight;
	Vector bottomLeft;
	Vector bottomRight;
	
	float left;
	float right;
	float top;
	float bot;

	bool visible;

};