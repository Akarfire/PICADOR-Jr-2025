#include <vector>
#include "Vector3.h"


class Grid
{
    // Resolution (number of nodes per axis)
	int resolutionX, resolutionY;

	// Dimensions of a single grid cell
	double deltaX, deltaY;

	// Physical coordinates of the origin
	Vector3 origin;

public:
    Grid(int resolutionX_, int resolutionY_, double deltaX_, double deltaY_, Vector3 origin_) 
    {

    }

    virtual ~Grid() {}


    int getCell_ID(const Vector3& location);
};
