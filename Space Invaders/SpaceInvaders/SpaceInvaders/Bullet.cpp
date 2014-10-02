#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Bullet.h"

Bullet::Bullet(GLuint textureID, float u, float v, float width, float height, float x, float y, float scale, bool onScreen):
	textureID(textureID), u(u), v(v), width(width), height(height), x(x), y(y), scale(scale), speed(1.0f), onScreen(onScreen){}

//draws the bullets
void Bullet::draw(){
	if(timer > 0){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glLoadIdentity();
		glTranslatef(x, y, 0.0f);

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
		glDisable(GL_BLEND);
	}
}

