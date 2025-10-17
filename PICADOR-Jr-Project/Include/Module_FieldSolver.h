#pragma once

#include "Module.h"

class FieldSolver : public Module
{
public:
    FieldSolver(PicadorJrCore* core_): Module(core_) {}

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