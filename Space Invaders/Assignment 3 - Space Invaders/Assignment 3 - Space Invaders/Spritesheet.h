#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Spritesheet{
public:
	
	Spritesheet();
	Spritesheet(unsigned int ID, float u, float v, float width, float height);

	void draw(float x, float y, float rotation, float scale);

	GLuint textureID;
	float u;
	float v;
	float width;
	float height;

};
