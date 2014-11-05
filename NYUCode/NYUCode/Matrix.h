//Nick Zheng
//Assignment 6 - Asteroids
#pragma once
#include "Vector.h"

using namespace std;

class Matrix{
public:
	
	Matrix();
	Matrix(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float n, float o, float p, float q);

	union{
		float m[4][4];
		float ml[16];
	};

	void identity();

	Matrix inverse();
	Matrix operator * (const Matrix& m2);
	Vector operator * (const Vector& v2);

};