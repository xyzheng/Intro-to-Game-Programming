#include "Entity.h"

Entity::Entity(unsigned int spriteSheet, float x, float y, float rotate, float width, float height)
		:textureID(spriteSheet), x(x), y(y), rotation(rotate), width(width), height(height), dynamic(true), collide(false){}

void Entity::draw(int spriteTexture, int index, int spriteCountX, int spriteCountY){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	GLfloat quad[] = { -width, height, -width, -height,
		width, -height, width, height};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	float u = (float)(((int)index) % spriteCountX) / (float) spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float) spriteCountY;
	float spriteWidth = 1.0f/(float)spriteCountX;
	float spriteHeight = 1.0f/(float)spriteCountY;

	GLfloat quadUVs[] = {u, v,
						 u, v+spriteHeight,
						 u+spriteWidth, v+spriteHeight,
						 u+spriteWidth, v};
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
//	glPopMatrix();
}