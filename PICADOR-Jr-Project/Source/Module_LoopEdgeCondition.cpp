#include "Module_LoopEdgeCondition.h"
#include "PicadorJrCore.h"

// Called by the core before entering the simulation loop
ModuleExecutionStatus LoopEdgeCondition::onBegin()
{
    if (core->getParticleGrid()->getPadding() > 0) return ModuleExecutionStatus::Success;
    else return ModuleExecutionStatus::Error;
}

// Called on every iteration of the simulation loop
ModuleExecutionStatus LoopEdgeCondition::onUpdate()
{
    return ModuleExecutionStatus::Success;
}