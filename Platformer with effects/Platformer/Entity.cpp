//Nick Zheng
//Assignment 4 - Single Screen Platformer WITH SOUND

#include "Entity.h"

Entity::Entity(unsigned int spritesheet, float x, float y, float xD, float yD, float rotate, float u, 
	float v, float width, float height, float mass, bool dynamic, bool collide):
	textureID(spritesheet), x(x), y(y), xD(xD), yD(yD), rotation(rotate), u(u), v(v), width(width), height(height),
		mass(mass), dynamic(true), collide(false), visible(true){}

void Entity::drawSprite(float scale){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	GLfloat quad[] = { -width * scale, height * scale, -width * scale, -height * scale,
		width * scale, -height * scale, width * scale, height * scale };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

bool Entity::collideX(Entity* entity){

	if((x + width*0.4f > entity->x - entity->width*0.4f && x + width*0.4f < entity->x + entity->width*0.4f) && 
		((y - height*0.4f > entity->y - entity->height*0.4f && y - height*0.4f < entity->y + entity->height*0.4f) ||
		(y + height*0.4f > entity->y - entity->height*0.4f && y + height*0.4f < entity->y - entity->height*0.4f))){
			collidedRight = true;
			return true;
	}
	
	else if((x - width*0.4f > entity->x + entity->width*0.4f && x - width*0.4f < entity->x - entity->width*0.4f) && 
			((y - height*0.4f > entity->y - entity->height*0.4f && y - height*0.4f < entity->y + entity->height*0.4f) ||
			(y + height*0.4f > entity->y - entity->height*0.4f && y + height*0.4f < entity->y - entity->height*0.4f))){
				collidedLeft = true;
				return true;
		}
	return false;
}

bool Entity::collideY(Entity* entity){

	if((y + height*0.4f > entity->y - entity->height*0.4f && y + height*0.4f < entity->y + entity->height*0.4f) &&
		((x - width*0.4f < entity->x + entity->width*0.4f && x - width*0.4f > entity->x - entity->width*0.4f) ||
		(x + width*0.4f > entity->x - entity->width*0.4f && x + width*0.4f < entity->x + entity->width*0.4f))){
			collidedTop = true;
			return true;
	}
	else if((y - height*0.4f > entity->y - entity->height*0.4f && y - height*0.4f < entity->y + entity->height*0.4f) &&
		((x - width*0.4f < entity->x + entity->width*0.4f && x - width*0.4f > entity->x - entity->width*0.4f) ||
		(x + width*0.4f > entity->x - entity->width*0.4f && x + width*0.4f < entity->x + entity->width*0.4f))){
			collidedBot = true;
			return true;
	}
	return false;
}