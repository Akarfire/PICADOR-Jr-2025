#pragma once

#include "Module.h"
#include "PicadorJrCore.h"

#include <functional>
#include <random>

class FieldGenerator : public Module
{
public:

protected:
    // List of generation profiles
    std::function<Vector3(Vector3)> E_Function;
    std::function<Vector3(Vector3)> B_Function;
    std::function<Vector3(Vector3)> J_Function;

public:
    FieldGenerator(PicadorJrCore* core_,    std::function<Vector3(Vector3)> E_Function_ = [](Vector3) { return Vector3::Zero; },
                                            std::function<Vector3(Vector3)> B_Function_ = [](Vector3) { return Vector3::Zero; },
                                            std::function<Vector3(Vector3)> J_Function_ = [](Vector3) { return Vector3::Zero; }
                                        ): Module(core_), E_Function(E_Function_), B_Function(B_Function_), J_Function(J_Function_) {}

    // Called by the core before entering the simulation loop
    virtual ModuleExecutionStatus onBegin() override;

    // Called on every iteration of the simulation loop
    virtual ModuleExecutionStatus onUpdate() override { return ModuleExecutionStatus::Success; };

    // Called after the core has exited the simulation loop
    virtual ModuleExecutionStatus onEnd() override { return ModuleExecutionStatus::Success; }
};