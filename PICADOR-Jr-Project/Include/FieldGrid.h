#pragma once

#include "Grid.h"

struct FieldGridNode
{
	// Electric field value
	Vector3 E;

	// Magnetic field value
	Vector3 B;

	// Current value
	Vector3 I;

	FieldGridNode() {}
};


class FieldGrid: public Grid
{
	// FieldGrid node array
	std::vector<FieldGridNode> nodeArray;

public:
	// Constructor
	FieldGrid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, const Vector3& origin_): Grid(resolutionX_, resolutionY_, deltaX_, deltaY_, origin_) {}

	// Interface

	// Returns an editable reference to the specified field node
	FieldGridNode& getNodeAt(size_t i, size_t j);

	// Returns interpolated values of fields in the given point
	FieldGridNode getFieldsAt(const Vector3& location) const;
};