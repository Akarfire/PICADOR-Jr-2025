#pragma once

#include "Grid.h"
#include "FieldContainer.h"


class FieldGrid: public Grid, public FieldContainer
{
	// FieldGrid node array
	std::vector<FieldData> nodeArray;

public:
	// Constructor
	FieldGrid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, const Vector3& origin_, size_t padding_);

	// Interface

	// Returns an editable reference to the specified field node
	FieldData& getNodeAt(GRID_INDEX i, GRID_INDEX j);
	const FieldData& getNodeAt(GRID_INDEX i, GRID_INDEX j) const;

	// Returns interpolated values of fields in the given point
	virtual const FieldData& getFieldsAt(const Vector3& location) const override;
};