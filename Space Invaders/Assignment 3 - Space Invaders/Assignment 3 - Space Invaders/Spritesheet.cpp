#include "Spritesheet.h"

Spritesheet::Spritesheet(){
}

Spritesheet::Spritesheet(unsigned int ID, float u, float v, float width, float height): 
	textureID(textureID), u(u), v(v), width(width), height(height){}

//draw sprite
void Spritesheet::draw(float x, float y, float rotation, float scale){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);

	GLfloat quad[] = {-width * scale, height * scale, 
					  -width * scale, -height * scale, 
					  width * scale, -height * scale, 
					  width * scale, height * scale };

	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = {u, v, 
						 u, v + height, 
						 u + width, v + height, 
						 u + width, v };

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

