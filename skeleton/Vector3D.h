#pragma once
#include "PxPhysicsAPI.h"
#include <cassert>
#include <cmath>

class Vector3D {
public:
	Vector3D() : x(0), y(0), z(0) {}
	Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3D(const physx::PxVec3& vec) : x(vec.x), y(vec.y), z(vec.z) {}

	float x, y, z; //los piden publicos explicitamente

	// copy assignment
	inline Vector3D& operator=(const Vector3D& v) noexcept
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	// ** various operations
	//

	// vector addition
	inline Vector3D operator+(const Vector3D& v) const
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	inline Vector3D& operator+=(const Vector3D& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	// subtraction 
	inline Vector3D operator-(const Vector3D& v) const
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	// mult by constant
	inline Vector3D operator*(float d) const
	{
		return Vector3D(x * d, y * d, z * d);
	}

	// division by constant (scaling)
	inline Vector3D operator/(float d) const
	{
		return Vector3D(x / d, y / d, z / d);
	}

	// equal
	inline bool operator==(const Vector3D& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	// not equal
	inline bool operator!=(const Vector3D& v) const
	{
		return !(*this == v);
	}

	// ** logic
	//

	// length of the vector
	inline float magnitude() const
	{
		return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
	}

	// vector in the same direction of length 1
	inline Vector3D normalize() const
	{
		float m = magnitude();
		assert(m > 0.000001f, "An operation is attempting to divide by zero."); // extra para saber si se divide entre 0
		if (m <= 0.000001f)
			return Vector3D(); // empty
		return *this / m;
	}
	// It returns a scalar value indicating vector alignment : positive(same direction,<90), negative(opposite
	// directions,>90), or zero(orthogonal,90) ABC=|A||B||C|cos(angle)
	inline float dot(const Vector3D& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	// (producto vectorial)
	Vector3D cross(const Vector3D& v) const {
		return Vector3D(
			y * v.z - z * v.y,
			-(x * v.z - z * v.x),
			x * v.y - y * v.x
		);
	}

	// ** conversions
	//
	
	// implicit 
	operator physx::PxVec3() const
	{
		return physx::PxVec3(x, y, z);
	}

};