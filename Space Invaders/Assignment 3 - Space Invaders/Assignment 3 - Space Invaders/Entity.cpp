#include "Entity.h"

//constructor
Entity::Entity(Spritesheet sprite, float scale, float x, float y, float width, float height, float rotation, float xDirection):
	sheetSprite(sprite), scale(scale), width(width), height(height), x(x), y(y), rotation(rotation), xDirection(xDirection), speed(1.0f), aliveStatus(true){}

//destructor
Entity::~Entity(){
}

//render the stuff
void Entity::render(){
	sheetSprite.draw(x, y, rotation, scale);
}