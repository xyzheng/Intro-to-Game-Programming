#include "Entity.h"

Entity::Entity(unsigned int spritesheet, float x, float y, float xD, float yD, float rotate, float u, 
	float v, float width, float height, float mass, bool dynamic):
	textureID(spritesheet), x(x), y(y), xD(xD), yD(yD), rotation(rotate), u(u), v(v), width(width), height(height),
		mass(mass), dynamic(true), visible(true){}

void Entity::draw(int spriteTexture, int index, int spriteCountX, int spriteCountY){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glPushMatrix();
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
	glPopMatrix();
}