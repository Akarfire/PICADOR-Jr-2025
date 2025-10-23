#pragma once

#include "Module.h"

class LoopEdgeCondition : public Module
{
public:
    LoopEdgeCondition(PicadorJrCore* core_): Module(core_) {}

    // Called by the core before entering the simulation loop
    virtual ModuleExecutionStatus onBegin() override;

    // Called on every iteration of the simulation loop
    virtual ModuleExecutionStatus onUpdate() override
    {
        return ModuleExecutionStatus::Success;
    }

    // Called after the core has exited the simulation loop
    virtual ModuleExecutionStatus onEnd() override { return ModuleExecutionStatus::Success; }
};