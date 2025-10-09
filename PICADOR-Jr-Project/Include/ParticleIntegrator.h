#pragma once

#include "Module.h"

class ParticleIntegrator : public Module
{
public:
    ParticleIntegrator(PicadorJrCore* core_): Module(core_) {}

    virtual ModuleExecutionStatus runModule() override
    {
        return ModuleExecutionStatus::Success;
    }
};