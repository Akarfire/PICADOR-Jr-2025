#pragma once

#include "Module.h"
#include "Grid.h"

class ParticleLoopEdgeCondition : public Module
{
    // Transfers particles of this cell onto the other end of the grid
    void processPaddingCell(GRID_INDEX i, GRID_INDEX j);

    // Given a padding cell, calculates indeces of the corresponding looping cell on the main grid
    std::pair<GRID_INDEX, GRID_INDEX> calculateLoopingCell(GRID_INDEX i, GRID_INDEX j);

public:
    ParticleLoopEdgeCondition(PicadorJrCore* core_): Module(core_) {}

    // Called by the core before entering the simulation loop
    virtual ModuleExecutionStatus onBegin() override;

    // Called on every iteration of the simulation loop
    virtual ModuleExecutionStatus onUpdate() override;

    // Called after the core has exited the simulation loop
    virtual ModuleExecutionStatus onEnd() override { return ModuleExecutionStatus::Success; }
};