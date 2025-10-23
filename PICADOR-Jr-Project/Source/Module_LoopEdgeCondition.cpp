#include "Module_LoopEdgeCondition.h"
#include "PicadorJrCore.h"

// Transfers particles of this cell onto the other end of the grid
void LoopEdgeCondition::processPaddingCell(GRID_INDEX i, GRID_INDEX j)
{

}

// Called by the core before entering the simulation loop
ModuleExecutionStatus LoopEdgeCondition::onBegin()
{
    if (core->getParticleGrid()->getPadding() > 0) return ModuleExecutionStatus::Success;
    else return ModuleExecutionStatus::Error;
}

// Called on every iteration of the simulation loop
ModuleExecutionStatus LoopEdgeCondition::onUpdate()
{
    // Loops over padding cells and calls processPaddingCell on them

    return ModuleExecutionStatus::Success;
}