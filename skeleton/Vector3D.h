#pragma once
#include "PxPhysicsAPI.h";
#include <cassert>
#include <cmath>

class Vector3D {
public:
	Vector3D::Vector3D() : _x(0), _y(0), _z(0) {}
	Vector3D::Vector3D(float x, float y, float z) : _x(x), _y(y), _z(z) {}
	Vector3D(const Vector3D& v) noexcept : _x(v.getX()), _y(v.getY()), _z(v.getZ()) {}
	Vector3D(Vector3D&& v) noexcept : _x(v.getX()), _y(v.getY()), _z(v.getZ()) {}
	Vector3D::Vector3D(physx::PxVec3 vec) : _x(vec.x), _y(vec.y), _z(vec.z) {}
	~Vector3D() {}

	inline float getX() const { return _x; }

	inline float getY() const { return _y; }

	inline float getZ() const { return _z; }

	// various setters
	inline void setX(float x) { _x = x; }
	inline void setY(float y) { _y = y; }
	inline void setZ(float z) { _z = z; }

	inline void set(float x, float y, float z)
	{
		_x = x;
		_y = y;
		_z = z;
	}

	inline void set(const Vector3D& v)
	{
		_x = v._x;
		_y = v._y;
		_z = v._z;
	}

	// copy assignment
	inline Vector3D& operator=(const Vector3D& v) noexcept
	{
		_x = v._x;
		_y = v._y;
		_z = v._z;
		return *this;
	}

	// v[0] is the first coordinate and v[1] is the second
	inline float& operator[](int i) noexcept
	{
		assert(i == 0 || i == 1 || i == 2);
		switch (i)
		{
		case 0:
			return _x;
			break;
		case 1:
			return _y;
			break;
		case 2:
			return _z;
			break;
		}
	}

	// v[0] is the first coordinate and v[1] is the second
	inline const float& operator[](int i) const noexcept
	{
		assert(i == 0 || i == 1 || i == 2);
		switch (i)
		{
		case 0:
			return _x;
			break;
		case 1:
			return _y;
			break;
		case 2:
			return _z;
			break;
		}
	}

	// ** various operations
	//

	// vector addition
	inline Vector3D operator+(const Vector3D& v) const
	{
		return Vector3D(_x + v._x, _y + v._y, _z + v._z);
	}

	inline Vector3D operator-(const Vector3D& v) const
	{
		return Vector3D(_x - v._x, _y - v._y, _z - v._z);
	}

	// mult by constant
	inline Vector3D operator*(float d) const
	{
		return Vector3D(_x * d, _y * d, _z * d);
	}
	//mult (escalar)
	inline float operator*(const Vector3D& d) const
	{
		return d._x * _x + d._y * _y + d._z * _z;
	}


	// division by constant (scaling)
	inline Vector3D operator/(float d) const
	{
		return Vector3D(_x / d, _y / d, _z / d);
	}

	// equal
	inline bool operator==(const Vector3D& v) const
	{
		return _x == v._x && _y == v._y && _z == v._z;
	}

	// not equal
	inline bool operator!=(const Vector3D& v) const
	{
		return !(*this == v);
	}

	// length of the vector
	inline float magnitude() const
	{
		return sqrtf(powf(_x, 2) + powf(_y, 2) + powf(_z, 2));
	}

	// vector in the same direction of length 1
	inline Vector3D normalize() const
	{
		float m = magnitude();
		assert(m > 0.000001f); // extra para saber si se divide entre 0
		if (m <= 0.000001f)
			return Vector3D(); // empty
		return *this / magnitude();
	}
	// It returns a scalar value indicating vector alignment : positive(same direction,<90), negative(opposite
	// directions,>90), or zero(orthogonal,90) ABC=|A||B||C|cos(angle)
	inline float dot(const Vector3D& other) const
	{
		return _x * other._x + _y * other._y + _z * other._z;
	}

	//producto vectorial
	Vector3D cross(const Vector3D& v) const {
		return Vector3D(
			_y * v._z - _z * v._y,
			-(_x * v._z - _z * v._x),
			_x * v._y - _y * v._x
		);
	}
private:
	float _x, _y, _z;

};