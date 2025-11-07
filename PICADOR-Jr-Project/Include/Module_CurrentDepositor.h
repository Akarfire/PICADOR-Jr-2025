#pragma once

#include "Module.h"
#include "FieldGrid.h"
#include "PicadorJrCore.h"
#include "FieldLoopEdgeCondition.h"

class CurrentDepositor : public Module
{
    FieldGrid* fieldGrid;
    ParticleGrid* particleGrid;

    FieldLoopEdgeCondition edgeCondition; //��� ������, ��� �������� � ������� ����� ��� ��������� ��� ������ ��� ��� ����������

public:
    CurrentDepositor(PicadorJrCore* core_): Module(core_) {}

    // Called by the core before entering the simulation loop
    virtual ModuleExecutionStatus onBegin() override;

    // Called on every iteration of the simulation loop
    virtual ModuleExecutionStatus onUpdate() override;

    // Called after the core has exited the simulation loop
    virtual ModuleExecutionStatus onEnd() override { return ModuleExecutionStatus::Success; }

    //deposition of particles in one cell
    void deposite(GRID_INDEX i, GRID_INDEX j);
};