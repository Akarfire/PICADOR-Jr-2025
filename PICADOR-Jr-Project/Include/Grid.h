#pragma once

#include <vector>
#include <stdexcept>
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
    Grid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, Vector3 origin_): resolutionX(resolutionX_), resolutionY(resolutionY_), deltaX(deltaX_), deltaY(deltaY_), origin(origin_) {}

    virtual ~Grid() {}

    // Я подумал и понял, что для линейного ID чтобы искать соседние узлы сверху (а их искать всегда нужно), пришлось бы всё равно индексы пересчитывать, поэтому пусть всё-таки возвращает i и j клетки
    // Returns coordinates of the bottom left corner of the cell, which the given point is in
    std::pair<size_t, size_t> getCell(const Vector3& location) const
    {
        std::pair<size_t, size_t> cell;
        cell.first = floor(location.x / deltaX);
        cell.second = floor(location.y / deltaY);
        return cell;
    }

    
    // Data getters

    size_t getResolutionX() const { return resolutionX; }
    size_t getResolutionY() const { return resolutionY; }

    double getDeltaX() const { return deltaX; }
    double getDeltaY() const { return deltaY; }

    const Vector3& getOrigin() const { return origin; }
};
