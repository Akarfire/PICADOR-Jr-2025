#pragma once

#include "Module.h"

class CurrentDepositor : public Module
{
public:
    CurrentDepositor(PicadorJrCore* core_): Module(core_) {}

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