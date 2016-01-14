#ifndef _VEC_
#define _VEC_

#include <math.h>

template <class T>
T clamp(T x, T min, T max)
{
	return x<min ? min : (x>max ? max : x);
}


template <class T>
T lerp(T a, T b, float bal)
{
	bal = clamp<float>(bal, 0, 1);
	return a + (b - a) * bal;
}


class vec{
public:
	// Share memory to allow different ways of accessing the same data
	union
	{
		struct
		{
			float x, y, z;
		};
		float v[3];
	};

	// Default constructor
	vec(float nx = 0, float ny = 0, float nz = 0) : x(nx), y(ny), z(nz) {}

	// Copy constructor
	vec(const vec &V) : x(V.x), y(V.y), z(V.z) {}

	// Negative
	friend vec operator-(const vec &V)
	{
		return vec(-V.x, -V.y, -V.z);
	}

	// Assignment operator
	vec &operator=(const vec &V)
	{
		x = V.x;
		y = V.y;
		z = V.z;

		return *this;
	}

	// Increment
	vec &operator+=(const vec &V)
	{
		x += V.x;
		y += V.y;
		z += V.z;

		return *this;
	}

	// Scalar multiplication
	vec &operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;

		return *this;
	}

	// Scalar division
	vec &operator/=(float s)
	{
		float f = 1 / s;

		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	// Addition
	friend vec operator+(const vec a, const vec b)
	{
		return vec(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	// Subtraction
	friend vec operator-(const vec a, const vec b)
	{
		return vec(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	// Cross product (vector product)
	friend vec operator*(const vec a, const vec b)
	{
		return vec(a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.z*b.y - a.y*b.x);
	}

	// Dot product (scalar product)
	friend float operator%(const vec a, const vec b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	// Scalar multiplication
	friend vec operator*(const vec V, float s)
	{
		return vec(V.x*s, V.y*s, V.z*s);
	}

	// Component multiplication
	void ComponentMul(const vec &V)
	{
		x *= V.x;
		y *= V.y;
		z *= V.z;
	}

	// Resize the vector to unit length
	void Normalize()
	{
		float s = 1 / Length();
		*this *= s;
	}

	// Make a normal of 3 given vertices
	void NormalOf(const vec &a, const vec &b, const vec &c)
	{
		vec u = b - c;
		vec v = b - a;
		*this = u * v;
	}

	// Returns the length of the vector
	float Length()
	{
		return sqrtf(x*x + y*y + z*z);
	}

	// Linear interpolation between two vectors
	void Lerp(const vec &a, const vec &b, float bal)
	{
		x = lerp<float>(a.x, b.x, bal);
		y = lerp<float>(a.y, b.y, bal);
		z = lerp<float>(a.z, b.z, bal);
	}
};

#endif