#include "FieldGrid.h"

FieldGrid::FieldGrid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, const Vector3& origin_, size_t padding_):
	Grid(resolutionX_, resolutionY_, deltaX_, deltaY_, origin_, padding_)
{
	this->nodeArray = std::vector<FieldData>((this->resolutionX + 2*this->padding) * (this->resolutionY+2*this->padding));
}

// Returns an editable reference to the specified field node
FieldData& FieldGrid::getNodeAt(GRID_INDEX i, GRID_INDEX j) 
{
	if (i < 0 || i >= resolutionX || j < 0 || j >= resolutionY) throw(std::runtime_error("Invalid field node index!"));
	return this->nodeArray[(i + padding) * resolutionX + (j + padding)];
}

const FieldData& FieldGrid::getNodeAt(GRID_INDEX i, GRID_INDEX j) const 
{
	if (i < 0 || i >= resolutionX || j < 0 || j >= resolutionY) throw(std::runtime_error("Invalid field node index!"));
	return this->nodeArray[(i + padding) * resolutionX + (j + padding)];
}

// Returns interpolated values of fields in the given point
FieldData FieldGrid::getFieldsAt(const Vector3& location) const
{
	size_t i = this->getCell(location).first, j = this->getCell(location).first;
	Vector3 cellOrigin(this->origin.x + i * this->deltaX, this->origin.y + j * this->deltaY, 0.0);
	Vector3 delta(this->deltaX, this->deltaY, 1.0);

	Vector3 newLocation = (location - cellOrigin) / delta;

	FieldData interpolated;
	interpolated.E = (this->getNodeAt(i, j)).E* (1 - newLocation.x) * (1 - newLocation.y) + (this->getNodeAt(i, j + 1)).E * (1 - newLocation.x) * newLocation.y + 
		(this->getNodeAt(i + 1, j)).E * newLocation.x * (1 - newLocation.y) + (this->getNodeAt(i + 1, j+1)).E *  newLocation.x * newLocation.y;

	interpolated.B = (this->getNodeAt(i, j)).B * (1 - newLocation.x) * (1 - newLocation.y) + (this->getNodeAt(i, j + 1)).B * (1 - newLocation.x) * newLocation.y +
		(this->getNodeAt(i + 1, j)).B * newLocation.x * (1 - newLocation.y) + (this->getNodeAt(i + 1, j + 1)).B * newLocation.x * newLocation.y;

	interpolated.J = (this->getNodeAt(i, j)).J * (1 - newLocation.x) * (1 - newLocation.y) + (this->getNodeAt(i, j + 1)).J * (1 - newLocation.x) * newLocation.y +
		(this->getNodeAt(i + 1, j)).J * newLocation.x * (1 - newLocation.y) + (this->getNodeAt(i + 1, j + 1)).J * newLocation.x * newLocation.y;

	return interpolated;
}