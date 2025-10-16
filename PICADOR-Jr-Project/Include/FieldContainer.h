#pragma once

#include "Vector3.h"

struct FieldData
{
	// Electric field value
	Vector3 E;

	// Magnetic field value
	Vector3 B;

	// Current value
	Vector3 J;

	FieldData() {}
};


class FieldContainer
{
public:

    FieldContainer() {}
    virtual ~FieldContainer() {}

    // Accesing field data at a given point in space
    virtual const FieldData& getFieldsAt(const Vector3& location) const = 0;
};