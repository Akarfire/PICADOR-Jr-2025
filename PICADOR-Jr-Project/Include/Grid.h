#include <vector>
#include "Vector3.h"


class Grid
{
protected:

    // Resolution (number of nodes per axis)
	size_t resolutionX, resolutionY;

	// Dimensions of a single grid cell
	double deltaX, deltaY;

	// Physical coordinates of the origin
	Vector3 origin;

public:
    Grid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, Vector3 origin_): resolutionX(resolutionX_), resolutionY(resolutionY_), deltaX(deltaX_), deltaY(deltaY_), origin(origin_) 
    {

    }

    virtual ~Grid() {}

    // Я подумал и понял, что для линейного ID чтобы искать соседние узлы сверху (а их искать всегда нужно), пришлось бы всё равно индексы пересчитывать, поэтому пусть всё-таки возвращает i и j клетки
    // Returns coordinates of the bottom left corner of the cell, which the given point is in
    std::pair<size_t, size_t> getCell(const Vector3& location);

    
    // Data getters

    size_t getResolutionX() const inline { return resolutionX; }
    size_t getResolutionY() const inline { return resolutionY; }

    double getDeltaX() const inline { return deltaX; }
    double getDeltaY() const inline { return deltaY; }

    const Vector3& getOrigin() const inline { return origin; }
};
