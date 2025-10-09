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
	FieldGrid(int resolutionX_, int resolutionY_, double deltaX_, double deltaY_, const Vector3& origin_): Grid(resolutionX_, resolutionY_, deltaX_, deltaY_, origin_) {}

	// Interface

	FieldGridNode& getNodeAt(int i, int j);

	const FieldGridNode& getFieldsAt(double x, double y) const;
	const FieldGridNode& getFieldsAt(const Vector3& coord) const;

	// TO DO: Add more getters for resolution, delta and edges (maybe)
};