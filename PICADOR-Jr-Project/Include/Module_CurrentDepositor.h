#pragma once

#include "Module.h"
#include "FieldGrid.h"

class CurrentDepositor : public Module
{
    FieldGrid* fieldGrid;

public:
    CurrentDepositor(PicadorJrCore* core_): Module(core_) {}

    virtual ModuleExecutionStatus onBegin() override
    {
        // Casting core's field container to FieldGrid
        fieldGrid = dynamic_cast<FieldGrid*>(core->getFieldContainer());

        return ModuleExecutionStatus::Success;
    }

    virtual ModuleExecutionStatus onUpdate() override
    {
        if (fieldGrid)
        {
            return ModuleExecutionStatus::Success;
        }

        return ModuleExecutionStatus::Error;
    }

    virtual ModuleExecutionStatus onEnd() override
    {
        return ModuleExecutionStatus::Success;
    }
};