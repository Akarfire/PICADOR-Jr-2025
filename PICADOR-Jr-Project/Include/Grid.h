#pragma once

#include <vector>
#include <stdexcept>
#include "Vector3.h"

#define GRID_INDEX int

class Grid
{
protected:

    // Resolution (number of nodes per axis)
	size_t resolutionX, resolutionY;

	// Dimensions of a single grid cell
    // может сделать Vector3 delta = {deltaX, deltaY, 1.0}
	double deltaX, deltaY;

	// Physical coordinates of the origin
	Vector3 origin;

    // Additional nodes beyound the resolution of the grid
    size_t padding;

public:
    Grid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, Vector3 origin_, size_t padding_): 
    resolutionX(resolutionX_), resolutionY(resolutionY_), deltaX(deltaX_), deltaY(deltaY_), origin(origin_), padding(padding_) {}

    virtual ~Grid() {}

    // Я подумал и понял, что для линейного ID чтобы искать соседние узлы сверху (а их искать всегда нужно), пришлось бы всё равно индексы пересчитывать, поэтому пусть всё-таки возвращает i и j клетки
    // Returns coordinates of the bottom left corner of the cell, which the given point is in
    std::pair<GRID_INDEX, GRID_INDEX> getCell(const Vector3& location) const
    {
        if (location.x < origin.x - padding * this->deltaX 
            || location.x > (origin.x + (this->resolutionX - 1 + padding)*this->deltaX) 
            || location.y < origin.y - padding * this->deltaY
            || location.y > (origin.y + (this->resolutionY - 1 + padding) * this->deltaY))
        {
            throw(std::runtime_error("Invalid location!"));
        }

        std::pair<GRID_INDEX, GRID_INDEX> cell;
        cell.second = (GRID_INDEX)floor((location.x - this->origin.x) / this->deltaX);
        cell.first = (GRID_INDEX)floor((location.y - this->origin.x) / this->deltaY);
        return cell;
    }

    
    // Data getters

    size_t getResolutionX() const { return resolutionX; }
    size_t getResolutionY() const { return resolutionY; }

    size_t getPadding() const { return padding; }

    double getDeltaX() const { return deltaX; }
    double getDeltaY() const { return deltaY; }

    const Vector3& getOrigin() const { return origin; }
};
