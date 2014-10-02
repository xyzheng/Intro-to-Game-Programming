#pragma once
#include "Spritesheet.h"

class Bullet{
public:

	Bullet();
	Bullet(Spritesheet sprite, float x, float y, float scale, float speed, float rotation);

	void render();

	float x;
	float y;
	float scale;
	float speed;
	float rotation;
	bool visible;
	
	Spritesheet sheetSprite;

};