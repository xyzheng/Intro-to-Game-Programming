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

Vector Vector::operator * (float num){
    Vector v;
    v.x = x * num;
    v.y = y * num;
    v.z = z * num;
    
    return v;
}

void Vector::operator += (const Vector& v){
	x += v.x;
	y += v.y;
	z += v.z;
}