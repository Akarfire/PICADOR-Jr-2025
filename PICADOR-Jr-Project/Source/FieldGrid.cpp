#include "FieldGrid.h"

FieldGrid::FieldGrid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, const Vector3& origin_, size_t padding_):
	Grid(resolutionX_, resolutionY_, deltaX_, deltaY_, origin_, padding_)
{
	// TO DO: Fill out the grid!
}

// Returns an editable reference to the specified field node
FieldData& FieldGrid::getNodeAt(size_t i, size_t j) 
{
	return this->nodeArray[(i + padding) * resolutionX + (j + padding)];
}

// Returns interpolated values of fields in the given point
const FieldData& FieldGrid::getFieldsAt(const Vector3& location) const 
{
	return FieldData();
}