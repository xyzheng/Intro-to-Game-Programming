//Nick Zheng
//Assignment 5 - Side Scroller

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SpriteSheet{
public:
	SpriteSheet();
	SpriteSheet(unsigned int textureID, int SpriteCountX, int SpriteCountY