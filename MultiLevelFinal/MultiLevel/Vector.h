#pragma once

class Vector{
public:

	Vector();
	Vector(float x, float y, float z);

	float length();
	void normalize();

	float x;
	float y;
	float z;

	Vector operator * (float num); 
	void operator += (const Vector& v); 

};