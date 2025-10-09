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
	FieldGrid(int inResolutionX, int inResolutionY, double inDeltaX, double inDeltaY, double originX, double originY);

	// Interface

	FieldGridNode& getNodeAt(int i, int j);

	const FieldGridNode& getFieldsAt(double x, double y) const;
	const FieldGridNode& getFieldsAt(const Vector3& coord) const;

	// TO DO: Add more getters for resolution, delta and edges (maybe)
};