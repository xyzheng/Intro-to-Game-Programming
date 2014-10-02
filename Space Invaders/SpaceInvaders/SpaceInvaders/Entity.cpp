#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"

//constructor
Entity::Entity(GLuint textureID, float u, float v, float width, float height, float scale):
	textureID(textureID), u(u), v(v), width(width), height(height), scale(scale), xDirect(1.0f), yDirect(0.0f), shoot(false){}

//destructor
Entity::~Entity(){
}

//draw the entity
void Entity::draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glScalef(width, height, 1.0);

	GLfloat quad[] = {-width * scale, height * scale,
					  -width * scale, -height * scale,
					  width * scale, -height * scale,
					  width * scale, height * scale
	};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = {u, v,
						 u, v + height,
						 u + width, v + height,
						 u + width, v
	};
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

}

//renders the entity if it's alive
void Entity::render(){
	if(aliveStatus == true){
		draw();
	}
}