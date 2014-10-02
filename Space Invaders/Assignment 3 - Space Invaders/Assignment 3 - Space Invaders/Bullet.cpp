#include "Bullet.h"

Bullet::Bullet(){
	x = 1.0f;
	y = 1.0f;
}

//constructor
Bullet::Bullet(Spritesheet sprite, float x, float y, float scale, float speed, float rotation):
	sheetSprite(sprite), x(x), y(y), scale(scale), speed(speed), rotation(rotation), visible(true){}

//render the bullets if they are visible
void Bullet::render(){
	if(visible){
		sheetSprite.draw(x, y, rotation, scale);
	}
}