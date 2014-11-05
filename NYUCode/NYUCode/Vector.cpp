//Nick Zheng
//Assignment 6 - Asteroids
#include "Vector.h"
#include <math.h>

//vector constructors
Vector::Vector(){
	x = 1.0f;
	y = 1.0f;
	z = 1.0f;
	normalize();
}

Vector::Vector(float x, float y, float z):
	x(x), y(y), z(z){}

//returns the length
float Vector::length(){
	float length = sqrt((x*x) + (y*y) + (z*z));
	return length;
}

//normalize vector
void Vector::normalize(){
	float theLength = length();
	x = x / theLength;
	y = y / theLength;
	z = z / theLength;
}