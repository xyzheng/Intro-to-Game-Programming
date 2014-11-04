//Nick Zheng
//Assignment 6 - Asteroids
#include "Entity.h"

Entity::Entity(unsigned int textureID, float u, float v, float width, float height, float xScale, float yScale):
	textureID(textureID), u(u), v(v), width(width), height(height), scale_x(xScale), scale_y(yScale), rotation(0.0f), xScale(1.0f), yScale(1.0f), visible(true){
		left = -width * 1.0f;
		right = width * 1.0f;
		top = height * 1.0f;
		bot = -height * 1.0f;
		topLeft = Vector(left, top, 1.0f);
		topRight = Vector(right, top, 1.0f);
		bottomLeft = Vector(left, bot, 1.0f);
		bottomRight = Vector(right, bot, 1.0f);}

void Entity::draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glPushMatrix();
	glMultMatrixf(matrix.ml);

	GLfloat quad[] = { -width * scale_x, height * scale_y, -width * scale_x, -height * scale_y,
		width * scale_x, -height * scale_y, width * scale_x, height * scale_y};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = {u, v, 
						 u, v + height, 
						 u + width, v + height, 
						 u + width, v};
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
}

void Entity::makeMatrix(){
	matrix = Matrix(xScale, 0.0f, 0.0f, 0.0f,
					0.0f, yScale, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);

	matrix = matrix * Matrix(cos(rotation), -sin(rotation), 0.0f, 0.0f,
							 sin(rotation), cos(rotation), 0.0f, 0.0f,
							 0.0f, 0.0f, 1.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 1.0f);

	matrix = matrix * Matrix(1.0f, 0.0f, 0.0f, x,
					         0.0f, 1.0f, 0.0f, y,
							 0.0f, 0.0f, 1.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 1.0f);

	inverse = matrix.inverse();
}

void Entity::update(){
	vector = Vector(float(cos(rotation+ M_PI/2.0f)), float(sin(rotation + M_PI/2.0f)), 0.0f);
	vector.x = vector.x * speed;
	vector.y = vector.y * speed;
	makeMatrix();
}

void Entity::generateRandom(){
	float speed = float(rand()) / float(RAND_MAX);
	rotation = float(rand()) / float(M_PI) * float(RAND_MAX) * 2.0f;
	float size = float(rand()) / float(RAND_MAX);
	xScale = size;
	yScale = size;
	vector = Vector(float(cos(rotation+ M_PI/2.0f)), float(sin(rotation + M_PI/2.0f)), 0.0f);
	makeMatrix();
	vector.x = speed;
	vector.y = speed;
}

