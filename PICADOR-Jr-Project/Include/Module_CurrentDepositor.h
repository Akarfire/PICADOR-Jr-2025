#pragma once

#include "Module.h"
#include "FieldGrid.h"
#include "PicadorJrCore.h"

class CurrentDepositor : public Module
{
    FieldGrid* fieldGrid;

public:
    CurrentDepositor(PicadorJrCore* core_): Module(core_) {}

    // Called by the core before entering the simulation loop
    virtual ModuleExecutionStatus onBegin() override;

    // Called on every iteration of the simulation loop
    virtual ModuleExecutionStatus onUpdate() override;

    // Called after the core has exited the simulation loop
    virtual ModuleExecutionStatus onEnd() override { return ModuleExecutionStatus::Success; }
};