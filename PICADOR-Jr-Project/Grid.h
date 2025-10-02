#pragma once

#include <vector>
#include "Vector3.h"

struct GridNode
{
	// Electric field value
	Vector3 E;

	// Magnetic field value
	Vector3 B;

	// Current value
	Vector3 I;

	GridNode() {}
};

class Grid
{
	// Grid resolution (number of nodes per axis)
	int resolutionX, resolutionY;

	// Dimensions of a single grid cell
	double deltaX, deltaY;

	// Physical coordinates of edges
	double edgeLeftX, edgeRightX, edgeTopY, edgeBottomY;

	// Grid node array
	std::vector<GridNode> nodeArray;


public:
	// Constructor
	Grid(int inResolutionX, int inResolutionY, double inDeltaX, double inDeltaY);

	// Interface

	GridNode& getNodeAt(int i, int j);

	const GridNode& getFieldsAt(double x, double y) const;
	const GridNode& getFieldsAt(const Vector3& coord) const;

	// TO DO: Add more getters for resolution, delta and edges (maybe)
};