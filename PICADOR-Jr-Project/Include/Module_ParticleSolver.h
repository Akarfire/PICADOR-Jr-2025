#pragma once

#include "Module.h"

struct Vector3;
struct Particle;
struct FieldData;

class ParticleSolver : public Module
{
protected:

    // Calculates new particle velocity based on the provided field value, using Boris's Method
    Vector3 CalculateNewParticleImpulse(const Particle& particle, const FieldData& field, double timeDelta);

public:
    ParticleSolver(PicadorJrCore* core_): Module(core_) {}

    virtual ModuleExecutionStatus onBegin() override { return ModuleExecutionStatus::Success; }

    // Called on every iteration of the simulation loop
    virtual ModuleExecutionStatus onUpdate() override;

    virtual ModuleExecutionStatus onEnd() override { return ModuleExecutionStatus::Success; }
};