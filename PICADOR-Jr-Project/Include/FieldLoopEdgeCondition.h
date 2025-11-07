#pragma once

#include <utility>

#include "FieldGrid.h"

class  FieldLoopEdgeCondition
{
public:

    std::pair<GRID_INDEX, GRID_INDEX> calculateLoopIndices(GRID_INDEX i, GRID_INDEX j);
};