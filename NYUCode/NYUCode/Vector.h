//Nick Zheng
//Assignment 6 - Asteroids
#pragma once
using namespace std;

class Vector{
public:

	Vector();
	Vector(float x, float y, float z);

	float length();
	void normalize();

	float x;
	float y;
	float z;

};