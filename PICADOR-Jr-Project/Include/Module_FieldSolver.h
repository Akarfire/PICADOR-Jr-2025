#pragma once

#include "Module.h"
#include "FieldGrid.h"
#include "PicadorJrCore.h"
#include "Constants.h"
#include "FieldLoopEdgeCondition.h"

class FieldSolver : public Module
{
    FieldGrid* fieldGrid;
    FieldLoopEdgeCondition edgeCondition; //вопрос аналогичный

    void updateE(GRID_INDEX i, GRID_INDEX j);
    void updateB(GRID_INDEX i, GRID_INDEX j);

public:
    FieldSolver(PicadorJrCore* core_): Module(core_) {}

    virtual ModuleExecutionStatus onBegin() override;

    virtual ModuleExecutionStatus onUpdate() override;

    virtual ModuleExecutionStatus onEnd() override
    {
        return ModuleExecutionStatus::Success;
    }
};