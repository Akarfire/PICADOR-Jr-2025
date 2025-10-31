#pragma once

#include "Module.h"
#include "FieldGrid.h"
#include "PicadorJrCore.h"
#include "Constants.h"

class FieldSolver : public Module
{
    FieldGrid* fieldGrid;

    void updateE(int i, int j);
    void updateB(int i, int j);

public:
    FieldSolver(PicadorJrCore* core_): Module(core_) {}

    virtual ModuleExecutionStatus onBegin() override;

    virtual ModuleExecutionStatus onUpdate() override;

    virtual ModuleExecutionStatus onEnd() override
    {
        return ModuleExecutionStatus::Success;
    }
};