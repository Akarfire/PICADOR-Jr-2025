#pragma once

struct Vector3
{
	// Coordinates
	double x, y, z;

	// Default constructor
	Vector3(double inX = 0.0, double inY = 0.0, double inZ = 0.0): x(inX), y(inY), z(inZ) {}

	// Data getters

	// Returns length of the vector
	double size();

	// Returns squared length of the vector (faster than normal .size())
	double sizeSquared();

	// Returns normalized vector (direction)
	Vector3 normalized();

	// Vector3 maskXY() { return Vector3(x, y, 0); }
	// void maskXY_InPlace() { z = 0; }

	// Operations
	const Vector3& operator*(double scale);

	const Vector3& operator+(const Vector3& rhs);
	const Vector3& operator-(const Vector3& rhs);
	const Vector3& operator*(const Vector3& rhs);
	const Vector3& operator/(const Vector3& rhs);

	// Scalar multiplication
	double dotProduct(const Vector3& rhs);

	// Vector multiplication
	Vector3 crossProduct(const Vector3& rhs);
};