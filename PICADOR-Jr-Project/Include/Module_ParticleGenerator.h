#pragma once

#include "Module.h"
#include "PicadorJrCore.h"

#include <functional>
#include <random>

class ParticleGenerator : public Module
{
public:

    struct ParticleGenerationProfile
    {
        // Sample particle to copy properties from, electron by default
        Particle sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);

        // Particle distribution density function
        std::function<size_t(Vector3)> particleDensityFunction = [](Vector3) { return 0; };

        // Temperature function (used for calculating initial speed)
        std::function<double(Vector3)> temperatureFunction = [](Vector3) { return 1e-40; };

        // Determines weight of the generated particles
        std::function<size_t(Vector3)> particleFactorFunction = [](Vector3) { return 1; };
    };

protected:

    // Random seed
    int seed;

    // Random engine used for normal destribution
    std::mt19937 randomEngine;

    // List of generation profiles
    std::vector<ParticleGenerationProfile> generationProfiles;

    // Generates particles for a single cell and appends them to outParticles
    void generateParticlesForCell(std::vector<Particle>& outParticles, GRID_INDEX cell_i, GRID_INDEX cell_j);

    // Generates initial velocity for a particle
    Vector3 generateParticleVelocity();

public:
    ParticleGenerator(PicadorJrCore* core_, int seed_): Module(core_), seed(seed_), randomEngine(seed) { srand(seed); }

    // Called by the core before entering the simulation loop
    virtual ModuleExecutionStatus onBegin() override;

    // Called on every iteration of the simulation loop
    virtual ModuleExecutionStatus onUpdate() override { return ModuleExecutionStatus::Success; };

    // Called after the core has exited the simulation loop
    virtual ModuleExecutionStatus onEnd() override { return ModuleExecutionStatus::Success; }


    // INTERFACE

    // Specifies different generation profiles
    void addGenerationProfile(const ParticleGenerationProfile& profile) { generationProfiles.push_back(profile); }
};