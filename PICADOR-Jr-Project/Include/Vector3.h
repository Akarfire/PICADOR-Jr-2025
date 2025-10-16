#pragma once

struct Vector3
{
	// Coordinates
	double x, y, z;

	// Default constructor
	Vector3(double x_, double y_, double z_): x(x_), y(y_), z(z_) {}
	Vector3(double xyz = 0.0): x(xyz), y(xyz), z(xyz) {}

	// Data getters

	// Returns length of the vector
	double size() const;

	// Returns squared length of the vector (faster than normal .size())
	double sizeSquared() const;

	// Returns normalized vector (direction)
	Vector3 normalized(double bias = 10e-5) const;

	// Vector3 maskXY() { return Vector3(x, y, 0); }
	// void maskXY_InPlace() { z = 0; }

	// Operations
	Vector3 operator*(double scale) const;

	Vector3 operator+(const Vector3& rhs) const;
	Vector3 operator-(const Vector3& rhs) const;
	Vector3 operator*(const Vector3& rhs) const;
	Vector3 operator/(const Vector3& rhs) const;

	// Scalar multiplication
	double dotProduct(const Vector3& rhs) const;

	// Vector multiplication
	Vector3 crossProduct(const Vector3& rhs) const;

	// Static version
	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 VectorMaskX;
	static const Vector3 VectorMaskY;
	static const Vector3 VectorMaskZ;
	static const Vector3 VectorMaskXY;
	static const Vector3 VectorMaskXZ;
	static const Vector3 VectorMaskYZ;
};