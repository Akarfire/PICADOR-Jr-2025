#pragma once

#include "Module.h"

class ParticleSolver : public Module
{
public:
    ParticleSolver(PicadorJrCore* core_): Module(core_) {}

    virtual ModuleExecutionStatus onBegin() override
    {
        return ModuleExecutionStatus::Success;
    }

    virtual ModuleExecutionStatus onUpdate() override
    {
        return ModuleExecutionStatus::Success;
    }

    virtual ModuleExecutionStatus onEnd() override
    {
        return ModuleExecutionStatus::Success;
    }
};