#include "FieldGrid.h"

// Returns an editable reference to the specified field node
FieldGridNode& FieldGrid::getNodeAt(size_t i, size_t j) {
	return this->nodeArray[i * resolutionX + j];
}

// Returns interpolated values of fields in the given point
FieldGridNode FieldGrid::getFieldsAt(const Vector3& location) const {
	return FieldGridNode();
}