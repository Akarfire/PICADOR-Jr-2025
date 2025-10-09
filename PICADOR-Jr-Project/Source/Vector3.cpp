#include "Vector3.h"

#include <math.h>

// Data getters

	// Returns length of the vector
	double Vector3::size() const
    {
        return sqrt(sizeSquared());
    }

	// Returns squared length of the vector (faster than normal .size())
	double Vector3::sizeSquared() const
    {
        return x*x + y*y + z*z;
    }

	// Returns normalized vector (direction)
	Vector3 Vector3::normalized(double bias) const
    {
        return (*this) / (size() + bias);
    }

	// Vector3 maskXY() { return Vector3(x, y, 0); }
	// void maskXY_InPlace() { z = 0; }

	// Operations
	Vector3 Vector3::operator*(double scale) const
    {
        Vector3 result;
        result.x = x * scale;
        result.y = y * scale;
        result.z = z * scale;

        return result;
    }

	Vector3 Vector3::operator+(const Vector3& rhs) const
    {
        Vector3 result;
        result.x = x + rhs.x;
        result.y = y + rhs.y;
        result.z = z + rhs.z;

        return result;
    }

	Vector3 Vector3::operator-(const Vector3& rhs) const
    {
        return (*this) + (rhs * -1);
    }

	Vector3 Vector3::operator*(const Vector3& rhs) const
    {
        Vector3 result;
        result.x = x * rhs.x;
        result.y = y * rhs.y;
        result.z = z * rhs.z;
    }

	Vector3 Vector3::operator/(const Vector3& rhs) const
    {
        Vector3 result;
        result.x = x / rhs.x;
        result.y = y / rhs.y;
        result.z = z / rhs.z;

        return result;
    }

	// Scalar multiplication
	double Vector3::dotProduct(const Vector3& rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

	// Vector multiplication
	Vector3 Vector3::crossProduct(const Vector3& rhs) const
    {
        Vector3 result;
        result.x = y * rhs.z - z * rhs.y;
        result.y = -1 * (x * rhs.z - z * rhs.x);
        result.z = x * rhs.y - y * rhs.x;

        return result;
    }