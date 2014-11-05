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
	float speed = float(-rand()) / float(RAND_MAX);
	rotation = float(rand()) / float(M_PI) * float(RAND_MAX);
	float size = float(rand()) / float(RAND_MAX);
	xScale = size;
	yScale = size;
	vector = Vector(float(cos(rotation+ M_PI/2.0f)), float(sin(rotation + M_PI/2.0f)), 0.0f);
	makeMatrix();
	vector.x = speed;
	vector.y = speed;
}

bool Entity::collided(Entity* e){
	//do the multiplication for the four points for each entity
	Vector a1 = matrix * e->topLeft;
	a1 = e->inverse * a1;
	Vector b1 = matrix * e->topRight;
	b1 = e->inverse * b1;
	Vector c1 = matrix * e->bottomLeft;
	c1 = e->inverse * c1;
	Vector d1 = matrix * e->bottomRight;
	d1 = e->inverse * d1;

	Vector a2 = matrix * e->topLeft;
	a2 = inverse * a2;
	Vector b2 = matrix * e->topRight;
	b2 = inverse * b2;
	Vector c2 = matrix * e->bottomLeft;
	c2 = inverse * c2;
	Vector d2 = matrix * e->bottomRight;
	d2 = inverse * d2; 

	//find smallest and largest points
	float eSmallX = min(min(a1.x, b1.x), min(c1.x, d1.x));
	float eSmallY = min(min(a1.y, b1.y), min(c1.y, d1.y));
	float eLargeX = max(max(a1.x, b1.x), max(c1.x, d1.x));
	float eLargeY = max(max(a1.y, b1.y), max(c1.y, d1.y));

	float e2SmallX = min(min(a2.x, b2.x), min(c2.x, d2.x));
	float e2SmallY = min(min(a2.y, b2.y), min(c2.y, d2.y));
	float e2LargeX = max(max(a2.x, b2.x), max(c2.x, d2.x));
	float e2LargeY = max(max(a2.y, b2.y), max(c2.y, d2.y));

	//collide
	if (eSmallX <= e->right && eLargeX >= e->left && eSmallY <= e->top && eLargeY >= e->bot && e2SmallX <= right && e2LargeX >= left && e2SmallY <= top && e2LargeY >= bot){
		if(x < e->x){
			x -= 0.01f;
			if (y < e->y){
				y -= 0.01f;
			}
			else if (y > e->y){
				y += 0.01f;
			}
		}
		else if(x > e->x){
			x += 0.01f;
			if(y < e->y){
				y -= 0.01f;
			}
			else if (y > e->y){
				y += 0.01f;
			}
		}
		else{
			if(y < e->y){
				y -= 0.01f;
			}
			else if(y > e->y){
				y += 0.01f;
			}
		}
		return true;
	}
	return false;
}

