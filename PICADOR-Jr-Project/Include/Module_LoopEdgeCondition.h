#pragma once

#include "Module.h"
#include "Grid.h"

class LoopEdgeCondition : public Module
{
    // Transfers particles of this cell onto the other end of the grid
    void processPaddingCell(GRID_INDEX i, GRID_INDEX j);

public:
    LoopEdgeCondition(PicadorJrCore* core_): Module(core_) {}

    // Called by the core before entering the simulation loop
    virtual ModuleExecutionStatus onBegin() override;

    // Called on every iteration of the simulation loop
    virtual ModuleExecutionStatus onUpdate() override;

    // Called after the core has exited the simulation loop
    virtual ModuleExecutionStatus onEnd() override { return ModuleExecutionStatus::Success; }
};