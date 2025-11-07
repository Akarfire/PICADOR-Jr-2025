#pragma once

#include <utility>

#include "FieldGrid.h"

class  FieldLoopEdgeCondition
{
private:
    std::pair<GRID_INDEX, GRID_INDEX> calculateLoopIndices(FieldGrid* fieldGrid, GRID_INDEX i, GRID_INDEX j);
    
public:
    
    void updateEEdge(FieldGrid* fieldGrid);

    void updateBEdge(FieldGrid* fieldGrid);

    void updateJEdge(FieldGrid* fieldGrid);
};